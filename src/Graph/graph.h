/**
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */
#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "../Util/comparator.h"
#include "../Util/index_t.h"

#define GRAPH_DEFAULT_SIZE 8

typedef struct _Graph Graph;

/**
 * Returns an empty graph with a default initial size.
 * @param data_size size of the data being stored
 * @param cmp comparator function for two elements on the graph
*/
Graph* graph_empty(size_t data_size, comparator_function_t cmp);

/**
 * Returns an empty graph with the given initial size.
 * @param data_size size of the data being stored
 * @param n_elements the initial size of the graph
 * @param cmp comparator function for two elements on the graph
*/
Graph* graph_init(size_t data_size, size_t n_elements, comparator_function_t cmp);

/**
 * Changes the comparator function of the graph
 * @param cmp the new comparator function
*/
void graph_configure(Graph *graph, comparator_function_t cmp);

/**
 * Fills the graph given arrays for vertices, edge sources, edge targets, and weights.
 * @param vertices_length the length of the vetrices array
 * @param edges the length of the sources, targets and weights array
*/
int graph_fill(Graph *graph, void *array_vertices, void *array_sources, void *array_targets, float *array_weights, size_t vertices_length, size_t edges_length);

/**
 * Adds a vertex to the graph.
 * @return 1 if the operation is successful
*/
int graph_add_vertex(Graph *graph, void *vertex);

/**
 * Adds [array_length] elements from array to the graph
*/
int graph_add_vertices_array(Graph *graph, void *array, size_t array_length);

/**
 * Removes a vertex from the graph.
 * @return 1 if the operation is successful
*/
int graph_remove_vertex(Graph *graph, void *vertex);

/**
 * Adds the first [array_length] of the array from the graph
*/
int graph_remove_vertices_array(Graph *graph, void *array, size_t array_length);

/**
 * @return true if the vertex exists in the graph
*/
bool graph_exists_vertex(Graph *graph, void *vertex);

/**
 * Adds an edge between source and target, with the given weight
 * @return 1 if the operation is successful
*/
int graph_add_edge(Graph *graph, void *source, void *target, float weight);

/**
 * Adds [arrays_length] edges to the graph.
 * Taking elements from array_sources, array_targets and array_weights.
 * @note The three arrays MUST BE THE SAME SIZE
*/
int graph_add_edges_array(Graph *graph, void *array_sources, void *array_targets, float *array_weights, size_t arrays_length);

/**
 * Removes the edge between source and target
 * @return 1 if the operation is successful
*/
int graph_remove_edge(Graph *graph, void *source, void *target);

/**
 * Removes the first [arrays_length] edges from the arrays.
 * @note The three arrays MUST BE THE SAME SIZE
*/
int graph_remove_edges_array(Graph *graph, void *array_sources, void *array_targets, size_t arrays_length);

/**
 * @return the weight of the edge between source and target
*/
float graph_get_edge(Graph *graph, void *source, void *target);

/**
 * @return true if an edge between source and target exists
*/
bool graph_exists_edge(Graph *graph, void *source, void *target);

/**
 * @return an index_t struct.
 * If the status is 1, the value is the index of the element in the graph.
 * If the status is != 1, this means the element is not present in the graph, and the value is garbage.
 * @note This is because the size_t type can't hold negative values, so we need these two elements (status and value) to cover all cases
*/
index_t graph_indexof(Graph *graph, void *vertex);

/**
 * @return the number of elements in the graph
*/
size_t graph_size(Graph *graph);

bool graph_isempty(Graph *graph);

/////// DIJKSTRA ///////

// Structure to return in the graph_dijkstra method. Holds the result of the algorithm.
typedef struct DijkstraData {
        float  *D;
        index_t *P;
        size_t n_elements;
        int status; // Flag to indicate the return status of the operation
} DijkstraData_t;

/**
 * Performs Dijkstra's Algorithms to compute the cheapest path from source to the 
 * rest of vertices in the graph. 
 * Returns a DijkstraData_t struct with the following elements.
 *      D (float*) an array of weights
 *      P (struct index_t) the pivot vertices (NOTE this is a struct, if the status is not SUCESS (1), this means there's no pivot)
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
void graph_free_dijkstra_data(DijkstraData_t *data);

///////////////////////////////////////////////////////////////////////////////

/////// FLOYD ///////

// Struct to return the result of floyd's algorithm
typedef struct FloydData {
        float **A; //  A matrix of weights
        index_t **P; // P matrixt of pivots
        size_t n_elements;
        int status; // represents the result status of the operation (1 if success)
} FloydData_t;

/**
 * Performs the Floyd–Warshall algorithm to find the cheapest path for all vertices of the graph.
*/
FloydData_t graph_floyd(Graph *graph);

/**
 * Prints to output the given FloydData_t
*/
void graph_print_floyd_data(FILE *output, FloydData_t data);

/**
 * Frees all memory allocated for the given FloydData_t
*/
void graph_free_floyd_data(FloydData_t *data);

/////////////////////////////////////////////////////////

//// OTHER ALGORITHMS ////

// Represents the Degree of a vertex
typedef struct vertexDegree {
        size_t deg_in;
        size_t deg_out;
        size_t deg;
        int status;
} vertexDegree_t;

/**
 * @note Returns a vertexDegree_t struct with the above information. 
 * @param     status:  if 1, this means the operation was successful and the rest of the elements are valid
 * @param     deg_out: the out degree of the vertex. This means, number of edges going into the vertex.
 * @param     deg_in:  the in degree of the vertex. This means, number of edges starting from the vertex.
 * @param     deg:     total degree of the vertex. It's value is deg_in plus deg_out.
*/
vertexDegree_t graph_get_degree(Graph *graph, void *vertex);

/**
 * @return true if the given vertex is a source vertex. 
 * @note A vertex is source if the in degree (number of edges entering the vertex) is 0
 *      and the out degree is > 0 (at least an edge is born from the vertex)
*/
bool graph_is_source_vertex(Graph *graph, void *vertex);

/**
 * @return true if the given vertex is a drain vertex. 
 * @note A vertex is source if the out degree (number of edges exiting the vertex) is 0
 *      and the in degree is > 0 (at least an edge eneters the vertex)
*/
bool graph_is_drain_vertex(Graph *graph, void *vertex);

/**
 * @return true if the given vertex is isolated.
 * @note A vertex is isolated if its degree is 0, wich means no 
 * edges go into the vertex, and no edges start from the vertex.
*/
bool graph_is_isolated_vertex(Graph *graph, void *vertex);

/**
 * @return The eccentricity of the vertex. This means, the highest of
 * the min cost paths between the vertex and the rest of vertices.
*/ 
float graph_eccentricity(Graph *graph, void *vertex);

/////////////////////////////////////////////////////////

////// Deep First Traverse //////////////////////////////
typedef struct traverse_df_data {
	void *elements;
	size_t elements_size;
        int status;
}traverse_df_data_t;

/**
 * Traverses the graph using the Deep First algorithm.
 * This means that it visits the "sons" of a graph first, and then the "brothers".
 * @param vertex to start the search in
 * @return A struct that contains an array with all the objects in the graph, traversed Deep First. This 
 * struct also contains a size_t variable "elements_size" that indicates the number of elements in the array. 
 * This is because, as there might be unreachable vertices, it's posible that the length of the array is < than the number of elements in the graph.
 * @note Remember to free the returned array when you finish working with it.
*/
traverse_df_data_t graph_traverse_DF(Graph *graph, void *vertex);

/////////////////////////////////////////////////////////

/**
 * Frees the graph
 * @return 1 if SUCESS
*/
int graph_free(Graph *graph);

/**
 * Resets the graph to it's original state.
 * @return the given graph, or NULL if something goes wrong
*/
Graph* graph_reset(Graph *graph);

#endif
