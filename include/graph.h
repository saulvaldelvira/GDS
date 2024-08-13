/*
 * graph.h - graph_t definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "./util/compare.h"

typedef struct graph_t graph_t;

/**
 * Initializes an empty graph.
 * @param data_size size of the data being stored
 * @param cmp comparator function for two elements on the graph
*/
graph_t* graph_init(size_t data_size, comparator_function_t cmp);

/**
 * Sets the comparator function of the graph
 * @param cmp the new comparator function
*/
void graph_set_comparator(graph_t *graph, comparator_function_t cmp);

/**
 * Sets the destructor function of the graph
 * @param destructor the new destructor function. NULL means no destructor
*/
void graph_set_destructor(graph_t *graph, destructor_function_t destructor);

/**
 * Fills the graph given arrays for vertices, edge sources, edge targets, and weights.
 * @param vertices_length the length of the vetrices array
 * @param edges the length of the sources, targets and weights array
*/
int graph_fill(graph_t *graph, void *array_vertices, void *array_sources, void *array_targets, float *array_weights, size_t vertices_length, size_t edges_length);

/**
 * Adds a vertex to the graph.
 * @return 1 if the operation is successful
*/
int graph_add_vertex(graph_t *graph, void *vertex);

/**
 * Adds [array_length] elements from array to the graph
*/
int graph_add_vertices_array(graph_t *graph, void *array, size_t array_length);

/**
 * Removes a vertex from the graph.
 * @note If it exists, the destructor will be called on the element.
 * @return 1 if the operation is successful
*/
int graph_remove_vertex(graph_t *graph, void *vertex);

/**
 * Adds the first [array_length] of the array from the graph
*/
int graph_remove_vertices_array(graph_t *graph, void *array, size_t array_length);

/**
 * @return true if the vertex exists in the graph
*/
bool graph_exists_vertex(graph_t *graph, void *vertex);

/**
 * Adds an edge between source and target, with the given weight
 * @return 1 if the operation is successful
*/
int graph_add_edge(graph_t *graph, void *source, void *target, float weight);

/**
 * Adds [arrays_length] edges to the graph.
 * Taking elements from array_sources, array_targets and array_weights.
 * @note The three arrays MUST BE THE SAME SIZE
*/
int graph_add_edges_array(graph_t *graph, void *array_sources, void *array_targets, float *array_weights, size_t arrays_length);

/**
 * Removes the edge between source and target
 * @return 1 if the operation is successful
*/
int graph_remove_edge(graph_t *graph, void *source, void *target);

/**
 * Removes the first [arrays_length] edges from the arrays.
*/
int graph_remove_edges_array(graph_t *graph, void *array_sources, void *array_targets, size_t arrays_length);

/**
 * Copies into dest the element at the given index
 * in the vertices array
*/
void* graph_vertex_at(graph_t *graph, ptrdiff_t index, void *dest);

/**
 * @return the weight of the edge between source and target
*/
float graph_get_edge(graph_t *graph, void *source, void *target);

/**
 * @return true if an edge between source and target exists
*/
bool graph_exists_edge(graph_t *graph, void *source, void *target);

/**
 * @return the index of the vertex in the graph
*/
ptrdiff_t graph_indexof(graph_t *graph, void *vertex);

/**
 * @return the number of elements in the graph
*/
size_t graph_size(graph_t *graph);

bool graph_isempty(graph_t *graph);

/////// DIJKSTRA ///////

/// Struct to return the result of dijkstra's algorithm
typedef struct {
        float     *D;     ///< Array of weights
        ptrdiff_t *P;     ///< Array of pivots
        uint32_t  n_elements;   ///< Number of elements in the D and P arrays
        int status;       ///< Return status of the algorithm
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
DijkstraData_t graph_dijkstra(graph_t *graph, void *source);

/**
 * Frees the allocated memory for the given DijkstraData_t's D and P arrays
*/
void graph_free_dijkstra_data(DijkstraData_t *data);

///////////////////////////////////////////////////////////////////////////////

/////// FLOYD ///////

/// Struct to return the result of floyd's algorithm
typedef struct {
        float     **A;   ///< Weights matrix
        ptrdiff_t **P;   ///< Pivots matrix
        uint32_t   n_elements;  ///< Dimension of the D and P matrices
        int status;      ///< Return status of the algorithm
} FloydData_t;

/**
 * Performs the Floyd–Warshall algorithm to find the cheapest path for all vertices of the graph.
*/
FloydData_t graph_floyd(graph_t *graph);

/**
 * Frees all memory allocated for the given FloydData_t
*/
void graph_free_floyd_data(FloydData_t *data);

/////////////////////////////////////////////////////////

//// OTHER ALGORITHMS ////

/// Represents the Degree of a vertex
typedef struct {
        uint16_t deg_in;   ///< Degree in the vertex
        uint16_t deg_out;  ///< Degree out the vertex
        int16_t  status;   ///< return status of the algorithm
} graph_degree;

/**
 * @note Returns a graph_degree struct with the above information.
 * @param     status:  if 1, this means the operation was successful and the rest of the elements are valid
 * @param     deg_out: the out degree of the vertex. This means, number of edges going into the vertex.
 * @param     deg_in:  the in degree of the vertex. This means, number of edges starting from the vertex.
 * @param     deg:     total degree of the vertex. It's value is deg_in plus deg_out.
*/
graph_degree graph_get_degree(graph_t *graph, void *vertex);

/**
 * @return true if the given vertex is a source vertex.
 * @note A vertex is source if the in degree (number of edges entering the vertex) is 0
 *      and the out degree is > 0 (at least an edge is born from the vertex)
*/
bool graph_is_source_vertex(graph_t *graph, void *vertex);

/**
 * @return true if the given vertex is a drain vertex.
 * @note A vertex is source if the out degree (number of edges exiting the vertex) is 0
 *      and the in degree is > 0 (at least an edge eneters the vertex)
*/
bool graph_is_drain_vertex(graph_t *graph, void *vertex);

/**
 * @return true if the given vertex is isolated.
 * @note A vertex is isolated if its degree is 0, wich means no
 * edges go into the vertex, and no edges start from the vertex.
*/
bool graph_is_isolated_vertex(graph_t *graph, void *vertex);

/**
 * @return The eccentricity of the vertex. This means, the highest of
 * the min cost paths between the vertex and the rest of vertices.
*/
float graph_eccentricity(graph_t *graph, void *vertex);

/////////////////////////////////////////////////////////

////// Traverse /////////////////////////////////////////

/**
 * Struct that hold the result of a graph traversal
*/
typedef struct {
        void *elements;         ///< Array of elements
        uint32_t elements_size;         ///< Number of elements in the array
        int status;                 ///< return status of the traversal
}graph_traversal;

/**
 * Traverses the graph using the Deep First algorithm.
 * This means that it visits the "sons" of a graph first, and then the "brothers".
 * @param vertex node to start the search in
 * @return A struct that contains an array with all the objects in the graph, traversed Deep First. This
 * struct also contains a size_t variable "elements_size" that indicates the number of elements in the array.
 * This is because, as there might be unreachable vertices, it's posible that the length of the array is < than the number of elements in the graph.
 * @note Remember to free the returned array when you finish working with it.
*/
graph_traversal graph_traverse_DF(graph_t *graph, void *vertex);

/**
 * Traverses the graph using the Breadth First algorithm.
 * This means that it visits the "brothers" of every node
 * before going one level deeper into the sons.
 * @param vertex node to start the search in
 * @return A struct that contains an array with all the objects in the graph, traversed Breadth First. This
 * struct also contains a size_t variable "elements_size" that indicates the number of elements in the array.
 * This is because, as there might be unreachable vertices, it's posible that the length of the array is < than the number of elements in the graph.
 * @note Remember to free the returned array when you finish working with it.
*/
graph_traversal graph_traverse_BF(graph_t *graph, void *vertex);

/////////////////////////////////////////////////////////

void graph_free(graph_t *g, ...);

/**
 * Frees all the given graphs.
 */
#define graph_free(...) graph_free(__VA_ARGS__, NULL)

/**
 * Resets the graph to it's original state.
*/
void graph_clear(graph_t *graph);

#ifdef __cplusplus
}
#endif

#endif
