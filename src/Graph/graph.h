/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdbool.h>
#include "../definitions.h"

typedef struct Graph {
        size_t n_elements;
        size_t max_elements;
        size_t data_size; // Size of the data type stored (in bytes)
        free_on_delete_t free_on_delete;
        int (*compare) (const void*, const void*);
        float **weights;
        bool **edges;
        void *nodes;
} Graph;



Graph graph_init(int size, size_t data_size, int (*cmp) (const void*, const void*));

static inline void graph_configure(Graph *graph, free_on_delete_t free_on_delete){
        graph->free_on_delete = free_on_delete;
}

int graph_add_node(Graph *graph, void *element);
int graph_remove_node(Graph *graph, void *element);
int graph_add_edge(Graph *graph, void *source, void *target, float weight);
int graph_remove_edge(Graph *graph, void *source, void *target);

bool graph_exists(Graph *graph, void *element);

#endif
