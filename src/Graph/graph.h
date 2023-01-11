/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 11-01-2023
 * Copyright (C) 2022-2023 Saúl Valdelvira
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdbool.h>
#include "../Util/comparator.h"

#define GRAPH_DEFAULT_SIZE 32

typedef struct _Graph Graph;

Graph* graph_empty(size_t data_size, comparator_function_t cmp);

Graph* graph_init(size_t data_size, size_t n_elements, comparator_function_t cmp);

int graph_add_node(Graph *graph, void *element);
int graph_remove_node(Graph *graph, void *element);
int graph_add_edge(Graph *graph, void *source, void *target, float weight);
int graph_remove_edge(Graph *graph, void *source, void *target);

bool graph_exists(Graph *graph, void *element);
size_t graph_n_elements(Graph *graph);
int graph_free(Graph *graph);

Graph* graph_reset(Graph *graph);

#endif
