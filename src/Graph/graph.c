/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "graph.h"
#include "../Util/checks.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int expand_memory(size_t size, size_t new_size, void ***nodes_ptr, float ***weights_ptr, bool ***edges_ptr){
        void** nodes = malloc( new_size * sizeof(void*));
        CHECK_MEMORY(nodes, expand_memory, ALLOCATION_ERROR)

        float **weights = malloc(new_size * sizeof(float*));
        CHECK_MEMORY(weights, expand_memory, ALLOCATION_ERROR)

        bool **edges = malloc(new_size * sizeof(bool*));
        CHECK_MEMORY(edges, expand_memory, ALLOCATION_ERROR)

        for (size_t i=0; i < size; i++){
                nodes[i] = (*nodes_ptr)[i];
                weights[i] = malloc(size * sizeof(float));
                CHECK_MEMORY(weights[i], expand_memory, ALLOCATION_ERROR)
                edges[i] =  malloc(size * sizeof(bool));
                CHECK_MEMORY(edges[i], expand_memory, ALLOCATION_ERROR)

                for (size_t j = 0; j < size; j++){
                        weights[i][j] = (*weights_ptr)[i][j];
                        edges[i][j] = (*edges_ptr)[i][j];
                }

                free((*weights_ptr)[i]);
                free((*edges_ptr)[i]);
        }

        for (size_t i=size; i < new_size; i++){
                nodes[i] = NULL;

                weights[i] = malloc(size * sizeof(float));
                CHECK_MEMORY(weights[i], expand_memory, ALLOCATION_ERROR)
                edges[i] =  malloc(size * sizeof(bool));
                CHECK_MEMORY(edges[i], expand_memory, ALLOCATION_ERROR)

                for (size_t j = 0; j < size; j++){
                        weights[i][j] = INFINITY;
                        edges[i][j] = false;
                }
        }

        free((*nodes_ptr));
        free((*weights_ptr));
        free((*edges_ptr));

        *nodes_ptr = nodes;
        *weights_ptr = weights;
        *edges_ptr = edges;
        return 1;
}

Graph graph_init(int (*cmp) (const void*, const void*)){
        void **nodes = NULL;
        float **weights = NULL;
        bool **edges = NULL;

        if (expand_memory(0, GRAPH_DEFAULT_SIZE,  &nodes, &weights, &edges) == ALLOCATION_ERROR){
                return (Graph){0};
        }

        return (Graph) {
                .n_elements = 0,
                .max_elements = GRAPH_DEFAULT_SIZE,
                .free_on_delete = DontFreeOnDelete,
                .compare = cmp,
                .weights = weights,
                .edges = edges,
                .nodes = nodes
        };
}

int graph_add_node(Graph *graph, void *element){
        CHECK_NULL(graph, graph_add_node, NULL_PARAMETER)
        if (graph->n_elements == graph->max_elements){
                graph->max_elements *= 2;
                if (expand_memory(graph->n_elements, graph->max_elements, &graph->nodes, &graph->weights, &graph->edges) == ALLOCATION_ERROR){
                        return ALLOCATION_ERROR;
                }
        }
        graph->nodes[graph->n_elements++] = element;
        return 1;
}

int graph_remove_node(Graph *graph, void *element);

int graph_add_edge(Graph *graph, void *source, void *target, float weight);
int graph_remove_edge(Graph *graph, void *source, void *target);


bool graph_exists(Graph graph, void *element){
        for (size_t i = 0; i < graph.n_elements; i++){
                if((*graph.compare) (element, graph.nodes[i]) == 0){
                        return true;
                }
        }
        return false;
}

void graph_free(Graph graph){
        free(graph.nodes);
        free(graph.edges);
        free(graph.weights);
}

int graph_reset(Graph *graph){
        graph_free(*graph);
        graph->n_elements = 0;
        graph->max_elements = GRAPH_DEFAULT_SIZE;
        graph->edges = NULL;
        graph->nodes = NULL;
        graph->weights = NULL;
        if (expand_memory(0, GRAPH_DEFAULT_SIZE,  &graph->nodes, &graph->weights, &graph->edges) == ALLOCATION_ERROR){
                return ALLOCATION_ERROR;
        }
        return 1;
}