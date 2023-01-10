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

#define GRAPH_DEFAULT_SIZE 32

typedef struct _Graph Graph;

Graph* graph_empty(size_t data_size, int (*cmp) (const void*, const void*));

Graph* graph_init(size_t data_size, size_t n_elements, int (*cmp) (const void*, const void*));

int graph_add_node(Graph *graph, void *element);
int graph_remove_node(Graph *graph, void *element);
int graph_add_edge(Graph *graph, void *source, void *target, float weight);
int graph_remove_edge(Graph *graph, void *source, void *target);

bool graph_exists(Graph *graph, void *element);
size_t graph_n_elements(Graph *graph);
int graph_free(Graph *graph);

Graph* graph_reset(Graph *graph);

#endif
