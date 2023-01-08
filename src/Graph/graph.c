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

Graph graph_init(int size, size_t data_size, int (*cmp) (const void*, const void*)){
        void* nodes = malloc(size * data_size);
        CHECK_MEMORY(nodes, graph_init, (Graph){})

        float **weights = malloc(size * sizeof(float*));
        CHECK_MEMORY(weights, graph_init, (Graph){})

        bool **edges = malloc(size * sizeof(bool*));
        CHECK_MEMORY(edges, graph_init, (Graph){})

        for (int i=0; i < size; i++){
                OFFSET(nodes, i, data_size) = NULL;

                weights[i] = malloc(size * sizeof(float));
                CHECK_MEMORY(weights[i], graph_init, (Graph){})
                edges[i] =  malloc(size * sizeof(bool));
                CHECK_MEMORY(edges[i], graph_init, (Graph){})

                for (int j = 0; j < size; j++){
                        weights[i][j] = INFINITY;
                        edges[i][j] = false;
                }
        }
        return (Graph) {
                .n_elements = 0,
                .max_elements = size,
                .free_on_delete = DontFreeOnDelete,
                .compare = cmp,
                .weights = weights,
                .edges = edges,
                .nodes = nodes
        };
}

int graph_add_node(Graph *graph, void *element){
        if (graph->n_elements == graph->max_elements){
                /// Reaclocatte memory
        }
        graph->nodes[graph->n_elements] = 
}

int graph_remove_node(Graph *graph, void *element);
int graph_add_edge(Graph *graph, void *source, void *target, float weight);
int graph_remove_edge(Graph *graph, void *source, void *target);


bool graph_exists(Graph graph, void *element){
        for (int i = 0; i < graph.n_elements; i++){
                if((*graph.compare) (element, graph.nodes[i]) == 0)
        }
}