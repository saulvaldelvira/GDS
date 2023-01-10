/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "graph.h"
#include "../Util/checks.h"
#include "../Util/definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "../Util/index_t.h"

struct _Graph {
        size_t n_elements;
        size_t max_elements;
        size_t data_size;
        int (*compare) (const void*, const void*);
        float *weights;
        char *edges;
        void *nodes;
};

static int expand_memory(size_t data_size, size_t old_size, size_t new_size, void **nodes_ptr, float **weights_ptr, char **edges_ptr){
        void *nodes = malloc( new_size * data_size);
        CHECK_MEMORY(nodes, expand_memory, ALLOCATION_ERROR)

        float *weights = malloc(new_size * new_size * sizeof(float));
        CHECK_MEMORY(weights, expand_memory, ALLOCATION_ERROR)

        char *edges = malloc(new_size * new_size * sizeof(char));
        CHECK_MEMORY(edges, expand_memory, ALLOCATION_ERROR)

        if(!memcpy(nodes, *nodes_ptr, old_size * data_size) || !memcpy(weights, *weights_ptr, old_size * sizeof(float)) || !memcpy(edges, *edges_ptr, old_size * sizeof(char))){
                fprintf(stderr, "ERROR: Could not expand graph\n");
                return -1;
        }

        memset(edges + (old_size * sizeof(char)), 0, new_size - (old_size * sizeof(char)));

        float *tmp = weights + old_size;
        for (size_t i = old_size; i < new_size; i++, tmp++){
                *tmp = INFINITY;
        }

        free((*nodes_ptr));
        free((*weights_ptr));
        free((*edges_ptr));

        *nodes_ptr = nodes;
        *weights_ptr = weights;
        *edges_ptr = edges;
        return 1;
}

Graph* graph_empty(size_t data_size, int (*cmp) (const void*, const void*)){
        CHECK_DATA_SIZE(data_size, graph_init, NULL)
        return graph_init(data_size, GRAPH_DEFAULT_SIZE, cmp);
}

Graph* graph_init(size_t data_size, size_t n_elements, int (*cmp) (const void*, const void*)){
        CHECK_DATA_SIZE(data_size, graph_init, NULL)
        CHECK_NULL(cmp, graph_init, NULL)
        Graph *graph = malloc(sizeof(Graph));

        void *nodes = NULL;
        float *weights = NULL;
        char *edges = NULL;

        if (expand_memory(data_size, 0, n_elements,  &nodes, &weights, &edges) == ALLOCATION_ERROR){
                return NULL;
        }

        graph->n_elements = 0;
        graph->max_elements = n_elements;
        graph->compare = cmp;
        graph->weights = weights;
        graph->edges = edges;
        graph->nodes = nodes;
        graph->data_size = data_size;
        return graph;
}

int graph_add_node(Graph *graph, void *element){
        CHECK_NULL(graph, graph_add_node, NULL_PARAMETER)
        CHECK_NULL(element, graph_add_node, NULL_PARAMETER)
        if (graph->n_elements == graph->max_elements){
                graph->max_elements *= 2;
                if (expand_memory(graph->data_size, graph->n_elements, graph->max_elements, &graph->nodes, &graph->weights, &graph->edges) == ALLOCATION_ERROR){
                        return ALLOCATION_ERROR;
                }
        }
        void *tmp = void_offset(graph->nodes, graph->n_elements * graph->data_size); // Position to copy value into
        tmp = memcpy(tmp, element, graph->data_size); // Perform the copy and store the reuslt
        CHECK_MEMORY_OP(tmp , graph_add_node , MEMORY_OP_ERROR) // Check for NULL result and return error if so
        graph->n_elements++;
        return 1;
}

static index_t indexof(Graph *graph, void *element){
        for (size_t i = 0; i < graph->n_elements; i++){
                if((*graph->compare)(void_offset(graph->nodes, i * graph->data_size), element) == 0){
                        return index_t(i);
                }
        }
        return INDEXT_NOT_FOUND;
}

int graph_remove_node(Graph *graph, void *element){
        CHECK_NULL(graph, graph_remove_node, NULL_PARAMETER)
        CHECK_NULL(element, graph_remove_node, NULL_PARAMETER)
        index_t index = indexof(graph, element);
        if (!index.status){
                return INDEX_OUT_OF_BOUNDS;
        }
        // Move latest node to this position
        if (index.value != graph->n_elements-1){
                // Set node at index to the last node in the array
                void *target = void_offset(graph->nodes, index.value * graph->data_size);
                void *source = void_offset(graph->nodes, (graph->n_elements-1) * graph->data_size);
                target = memmove(target, source, graph->data_size);
                CHECK_MEMORY_OP(target, graph_remove_node, MEMORY_OP_ERROR)

                target = (void*) (graph->weights + index.value);
                source = (void*) (graph->weights + graph->n_elements-1);

                target = memmove(target, source, graph->max_elements * sizeof(float));
                CHECK_MEMORY_OP(target, graph_remove_node, MEMORY_OP_ERROR)

                target = (void*) (graph->edges + index.value);
                source = (void*) (graph->edges + graph->n_elements-1);

                target = memmove(target, source, graph->max_elements * sizeof(char));
                CHECK_MEMORY_OP(target, graph_remove_node, MEMORY_OP_ERROR)

                for (size_t i = 0; i < graph->max_elements; i++){
                        float *f_target = matrix_offset(graph->weights, graph->max_elements, index.value, i);
                        float *f_source = matrix_offset(graph->weights, graph->max_elements, graph->max_elements-1, i);
                        *f_target = *f_source;
                        *f_source = INFINITY;

                        f_target = matrix_offset(graph->weights, graph->max_elements, i, index.value);
                        f_source = matrix_offset(graph->weights, graph->max_elements, i, graph->max_elements-1);

                        *f_target = *f_source;
                        *f_source = INFINITY;

                        char *c_target = matrix_offset(graph->edges, graph->max_elements, index.value, i);
                        char *c_source = matrix_offset(graph->edges, graph->max_elements, graph->max_elements-1, i);
                        *c_target = *c_source;
                        *c_source = 0;

                        c_target = matrix_offset(graph->edges, graph->max_elements, i, index.value);
                        c_source = matrix_offset(graph->edges, graph->max_elements, i, graph->max_elements-1);

                        *c_target = *c_source;
                        *c_source = 0;
                }

        }
        graph->n_elements--;
        return 1;
}

int graph_add_edge(Graph *graph, void *source, void *target, float weight);
int graph_remove_edge(Graph *graph, void *source, void *target);


bool graph_exists(Graph *graph, void *element){
        CHECK_NULL(element, graph_exists, false)
        for (size_t i = 0; i < graph->n_elements; i++){
                if((*graph->compare) (element, void_offset(graph->nodes, graph->data_size * i)) == 0){
                        return true;
                }
        }
        return false;
}

size_t graph_n_elements(Graph *graph){
        CHECK_NULL(graph, graph_n_elements, 0)
        return graph->n_elements;
}

int graph_free(Graph *graph){
        CHECK_NULL(graph, graph_free, NULL_PARAMETER)
        free(graph->nodes);
        free(graph->edges);
        free(graph->weights);
        free(graph);
        return 1;
}

Graph* graph_reset(Graph *graph){
        CHECK_NULL(graph, graph_reset, NULL)
        free(graph->nodes);
        free(graph->edges);
        free(graph->weights);
        graph->n_elements = 0;
        graph->max_elements = GRAPH_DEFAULT_SIZE;
        graph->edges = NULL;
        graph->nodes = NULL;
        graph->weights = NULL;
        if (expand_memory(graph->data_size, 0, GRAPH_DEFAULT_SIZE,  &graph->nodes, &graph->weights, &graph->edges) == ALLOCATION_ERROR){
                return NULL;
        }
        return graph;
}