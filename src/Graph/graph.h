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
#include <stdio.h>
#include "../Util/comparator.h"
#include "../Util/index_t.h"

#define GRAPH_DEFAULT_SIZE 32

typedef struct _Graph Graph;

Graph* graph_empty(size_t data_size, comparator_function_t cmp);

Graph* graph_init(size_t data_size, size_t n_elements, comparator_function_t cmp);

int graph_add_node(Graph *graph, void *element);
int graph_remove_node(Graph *graph, void *element);
bool graph_exists_node(Graph *graph, void *element);

int graph_add_edge(Graph *graph, void *source, void *target, float weight);
int graph_remove_edge(Graph *graph, void *source, void *target);
float graph_get_edge(Graph *graph, void *source, void *target);
bool graph_exists_edge(Graph *graph, void *source, void *target);

index_t graph_indexof(Graph *graph, void *element);
size_t graph_n_elements(Graph *graph);

// Algorithms

// Structure to return in the graph_dijkstra method. Holds the result of the algorithms.
typedef struct DijkstraData {
        float  *D;
        index_t *P;
        size_t n_elements;
        int status; // Flag to indicate the return status of the operation
} DijkstraData_t;

/**
 * Performs Dijkstra's Algorithms to compute the cheapest path from source to the 
 * rest of nodes in the graph. 
 * Returns a DijkstraData_t struct with the following elements.
 *      D (float*) an array of weights
 *      P (struct index_t) the pivot nodes (NOTE this is a struct, if the status is not SUCESS (1), this means there's no pivot)
 *      n_elements (size_t) the number of elements in the arrays
 *      status the return status of the operation. If the algorith does not encounter any issue, returns SUCESS (1)
 * @note REMEMBER to free the D and P arrays!! There's a fucntion graph_free_dijkstra_data that does that for you.
*/
DijkstraData_t graph_dijkstra(Graph *graph, void *source);

/**
 * Prints a DijkstraData_t struct to output
*/
void graph_print_dijkstra_data(FILE *output, DijkstraData_t data);

/**
 * Frees the allocated memory for the given DijkstraData_t's D and P arrays
*/
void graph_free_dijkstra_data(DijkstraData_t data);

int graph_free(Graph *graph);
Graph* graph_reset(Graph *graph);

#endif
