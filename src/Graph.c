/*
 * Graph.c - Graph implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "Graph.h"
#define GDS_ENABLE_ERROR_MACROS
#include "./util/error.h"
#include "./util/definitions.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

/**
 * Graph struct
 * @headerfile Graph.h <GDS/Graph.h>
 * @see Graph.h
*/
struct Graph {
	size_t n_elements;	///< Number of elements in the Graph
	size_t max_elements;	///< Current capacity of the graph
	size_t data_size;	///< Size (in bytes) of the data type
	comparator_function_t compare;		///< Comparator function pointer
	destructor_function_t destructor;	///< Destructor function pointer
	float **weights;	///< Weights matrix
	int8_t **edges;		///< Edges matrix
	void *vertices;		///< Vertices matrix
};

#define GRAPH_DEFAULT_SIZE 8

/// CONSTRUCTORS //////////////////////////////////////////////////////////////

/**
 * Expands the graph's number of elements to new_size. This means:
 * 1) Allocates new spaces for vertices, weights and edges.
 * 2) Copies old values into these new, bigger spaces.
 * 3) Fills rest of the new spaces with the default values (0 for edges and INFINITY for weights)
 *      In the case of edges, since we allocate them with calloc, the values are already 0 by default.
 * 4) Frees the old spaces
 * 5) Sets the graphs vertices, weights and edges to be this new spaces. Also update max_elements value to new_size
*/
static void expand_memory(Graph *graph, size_t new_size){
	// Allocate vertices
	void *vertices = malloc(new_size * graph->data_size);
	// Allocate weights
	float **weights = malloc(new_size * sizeof(*weights));
        // Allocate edges
	int8_t **edges = malloc(new_size * sizeof(*edges));
	assert(vertices && weights && edges);

	// Copy old vertex values in the range [0, oldSize) and free the old one
	memcpy(vertices, graph->vertices, graph->max_elements * graph->data_size);
	free(graph->vertices);

	// Initialize the columns for edges and weights.
	// We divide the loop in two ranges [0 - oldSize) and [oldSize - newSize)
	// In the first one, we allocate the new memmory and copy the old values.
	for (size_t i = 0; i < graph->max_elements; i++){
		// Allocate weights[i] and copy old values
		weights[i] = malloc(new_size * sizeof(*weights[i]));
		assert(weights[i]);
		memcpy(weights[i], graph->weights[i], sizeof(*weights[i])*graph->max_elements);

		// Allocate edges[i] and copy old values
		edges[i] = calloc(new_size, sizeof(*edges[i]));
		assert(edges[i]);
		memcpy(edges[i], graph->edges[i], sizeof(*edges[i])*graph->max_elements);

		// Set the new rows to INFINITY. Edges are already 0 because of calloc.
		for (size_t j = graph->max_elements; j < new_size; j++)
			weights[i][j] = INFINITY;

		// Free the old pointers
		free(graph->weights[i]);
		free(graph->edges[i]);
	}

	// In the second one [oldSize, newSize), we also allocate memory
	// but, since there are no old values to copy, we just set the default values
	for (size_t i = graph->max_elements; i < new_size; i++){
		weights[i] = malloc(new_size * sizeof(*weights[i]));
		assert(weights[i]);

		edges[i] = calloc(new_size, sizeof(*edges[i]));
		assert(edges[i]);

		for (size_t j = 0; j < new_size; j++){
			weights[i][j] = INFINITY;
		}
	}
	// Free old pointers
	free(graph->weights);
	free(graph->edges);

	graph->vertices = vertices;
	graph->weights = weights;
	graph->edges = edges;
	graph->max_elements = new_size;
}

Graph* graph_empty(size_t data_size, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param();
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	return graph_init(data_size, GRAPH_DEFAULT_SIZE, cmp);
}

Graph* graph_init(size_t data_size, size_t n_elements, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param();
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	Graph *graph = malloc(sizeof(*graph));
	assert(graph);

	graph->n_elements = 0;
	graph->max_elements = 0;
	graph->compare = cmp;
	graph->destructor = NULL;
	graph->weights = NULL;
	graph->edges = NULL;
	graph->vertices = NULL;
	graph->data_size = data_size;

	// "Expanding" from 0 to n_elements turns into just initializing
	// weights, edges and vertices to their default size and state.
	expand_memory(graph, n_elements);
	return graph;
}

void graph_set_comparator(Graph *graph, comparator_function_t cmp){
	if (!graph || !cmp)
		printerr_null_param();
	else
		graph->compare = cmp;
}

void graph_set_destructor(Graph *graph, destructor_function_t destructor){
	if (!graph)
		printerr_null_param();
	else
		graph->destructor = destructor;
}

int graph_fill(Graph *graph, void *array_vertices, void *array_sources, void *array_targets, float *array_weights, size_t vertices_length, size_t edges_length){
	if (!graph || !array_vertices || !array_sources || !array_targets || !array_weights){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	int status = graph_add_vertices_array(graph, array_vertices, vertices_length);
	if (status != SUCCESS)
		return status;
	status = graph_add_edges_array(graph, array_sources, array_targets, array_weights, edges_length);
	if (status != SUCCESS)
		return status;
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// vertices /////////////////////////////////////////////////////////////////////
int graph_add_vertex(Graph *graph, void *vertex){
	if (!graph || !vertex){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (graph->n_elements == graph->max_elements)
		expand_memory(graph, graph->max_elements * 2);
	void *tmp = void_offset(graph->vertices, graph->n_elements * graph->data_size);
	memcpy(tmp, vertex, graph->data_size);
	graph->n_elements++;
	return SUCCESS;
}

int graph_add_vertices_array(Graph *graph, void *array, size_t array_length){
	if (!graph || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (array_length-- > 0){
		int status = graph_add_vertex(graph, array);
		if (status != SUCCESS){
			return status;
		}
		array = void_offset(array, graph->data_size);
	}
	return SUCCESS;
}

/**
 * Removes a vertex from the graph. This means
 * To do so it swaps the elements to delete with the last elements in the array of vertices.
 * It also has to swap the edges and weights values.
 * After decrementing the n_elements values, the old (now "removed") vertex becomes garbage memory to be overwritten in the next add
*/
int graph_remove_vertex(Graph *graph, void *vertex){
	if (!graph || !vertex){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	ptrdiff_t index = graph_indexof(graph, vertex); // Get the index of the vertex
	if (index < 0)
		return index;
	// Move latest vertex to this position (only if it is not already the last)
	if ((size_t)index != graph->n_elements-1){
		// Set vertex at index to the last vertex in the array
		void *target = void_offset(graph->vertices, index * graph->data_size);
		void *source = void_offset(graph->vertices, (graph->n_elements-1) * graph->data_size);
		memmove(target, source, graph->data_size);

		// Swap the weights columns of the vertex to be removed and the last one
		target = (void*) (graph->weights[index]);
		source = (void*) (graph->weights[graph->n_elements-1]);
		memmove(target, source, graph->max_elements * sizeof(*graph->weights[0]));

		// Swap the edges columns of the vertex to be removed and the last one
		target = (void*) (graph->edges[index]);
		source = (void*) (graph->edges[graph->n_elements-1]);
		memmove(target, source, graph->max_elements * sizeof(*graph->edges[0]));

		// Swap rows of the vertex to be removed and the last one
		for (size_t i = 0; i < graph->max_elements; i++){
			graph->edges[i][index] = graph->edges[i][graph->n_elements-1];
			graph->weights[i][index] = graph->weights[i][graph->n_elements-1];

			// We also have to set the edges and weights to default to guarantee that in the next add, those edges and weights will not hold garbage
			graph->edges[i][graph->n_elements-1] = 0;
			graph->edges[graph->n_elements-1][i] = 0;
			graph->weights[i][graph->n_elements-1] = INFINITY;
			graph->weights[graph->n_elements-1][i] = INFINITY;
		}
	}else{
		for (size_t i = 0; i < graph->max_elements; i++){
			graph->edges[i][graph->n_elements-1] = 0;
			graph->edges[graph->n_elements-1][i] = 0;
			graph->weights[i][graph->n_elements-1] = INFINITY;
			graph->weights[graph->n_elements-1][i] = INFINITY;
		}
	}

	graph->n_elements--;
	return SUCCESS;
}

int graph_remove_vertices_array(Graph *graph, void *array, size_t array_length){
	if (!graph || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (array_length-- > 0){
		graph_remove_vertex(graph, array);
		array = void_offset(array, graph->data_size);
	}
	return SUCCESS;
}

bool graph_exists_vertex(Graph *graph, void *vertex){
	if (!graph || !vertex){
		printerr_null_param();
		return false;
	}
	void *tmp;
	for (size_t i = 0; i < graph->n_elements; i++){
		tmp = void_offset(graph->vertices, graph->data_size * i);
		if((*graph->compare) (vertex, tmp) == 0){
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////

///// EDGES ///////////////////////////////////////////////////////////////////

int graph_add_edge(Graph *graph, void *source, void *target, float weight){
	if (!graph || !source || !target){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	ptrdiff_t index_src = graph_indexof(graph, source);
	if (index_src < 0)
		return index_src;
	ptrdiff_t index_dst = graph_indexof(graph, target);
	if (index_dst < 0)
		return index_dst;
	graph->edges[index_src][index_dst] = 1;
	graph->weights[index_src][index_dst] = weight;
	return SUCCESS;
}

int graph_add_edges_array(Graph *graph, void *array_sources, void *array_targets, float *array_weights, size_t arrays_length){
	if (!graph || !array_sources || !array_targets || !array_weights){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (arrays_length-- > 0){
		int status = graph_add_edge(graph, array_sources, array_targets, *array_weights);
		if (status != SUCCESS){
			return status;
		}
		array_sources = void_offset(array_sources, graph->data_size);
		array_targets = void_offset(array_targets, graph->data_size);
		array_weights++;
	}
	return SUCCESS;
}

int graph_remove_edge(Graph *graph, void *source, void *target){
	if (!graph || !source || !target){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	ptrdiff_t index_src = graph_indexof(graph, source);
	if (index_src < 0)
		return index_src;
	ptrdiff_t index_dst = graph_indexof(graph, target);
	if (index_dst < 0)
		return index_dst;
	graph->edges[index_src][index_dst] = 0;
	graph->weights[index_src][index_dst] = INFINITY;
	return SUCCESS;
}

int graph_remove_edges_array(Graph *graph, void *array_sources, void *array_targets, size_t arrays_length){
	if (!graph || !array_sources || !array_targets){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (arrays_length-- > 0){
		graph_remove_edge(graph, array_sources, array_targets);
		array_sources = void_offset(array_sources, graph->data_size);
		array_targets = void_offset(array_targets, graph->data_size);
	}
	return SUCCESS;
}

float graph_get_edge(Graph *graph, void *source, void *target){
	if (!graph || !source || !target){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	ptrdiff_t index_src = graph_indexof(graph, source);
	if (index_src < 0)
		return index_src;
	ptrdiff_t index_dst = graph_indexof(graph, target);
	if (index_dst < 0)
		return index_dst;
	return graph->weights[index_src][index_dst];
}

bool graph_exists_edge(Graph *graph, void *source, void *target){
	if (!graph || !source || !target){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	ptrdiff_t index_src = graph_indexof(graph, source);
	if (index_src < 0)
		return index_src;
	ptrdiff_t index_dst = graph_indexof(graph, target);
	if (index_dst < 0)
		return index_dst;
	return graph->edges[index_src][index_dst];
}

///////////////////////////////////////////////////////////////////////////////

size_t graph_size(Graph *graph){
	if (!graph){
		printerr_null_param();
		return 0;
	}
	return graph->n_elements;
}

bool graph_isempty(Graph *graph){
	if (!graph){
		printerr_null_param();
		return false;
	}
	return graph->n_elements == 0;
}

ptrdiff_t graph_indexof(Graph *graph, void *vertex){
	for (size_t i = 0; i < graph->n_elements; i++){
		if((*graph->compare)(void_offset(graph->vertices, i * graph->data_size), vertex) == 0){
			return i;
		}
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

/// DIJKSTRA //////////////////////////////////////////////////////////////////

/**
 * Initializes the DijkstraData_t structure.
*/
static void graph_init_dijkstra(DijkstraData_t *dijkstra, Graph *graph, size_t source){
	dijkstra->D = malloc(graph->n_elements * sizeof(*dijkstra->D));
	dijkstra->P = malloc(graph->n_elements * sizeof(*dijkstra->P));
	assert(dijkstra->D && dijkstra->P);

	for (size_t i = 0; i < graph->n_elements; i++){
		dijkstra->D[i] = graph->weights[source][i];
		if (graph->edges[source][i]){
			dijkstra->P[i] = source;
		} else{
			dijkstra->P[i] = -1;
		}

	}
	// The source vertex's cheapest path is allways itself with weight 0
	dijkstra->D[source] = 0.0f;
	dijkstra->P[source] = source;
	dijkstra->n_elements = graph->n_elements;
	dijkstra->status = SUCCESS;
}

/**
 * Returns the next pivot for the algorithm.
 * The next pivot is the vertex with the lowest cost that has not been visited yet
 * @param S an array of visited vertices. 1 if visited, 0 if not visited
 * @param D an array of weights
 * @param n_elements the number of elements in the arrays
*/
static ptrdiff_t graph_get_pivot(uint8_t *S, float *D, size_t n_elements){
	ptrdiff_t pivot = -1;
	float min = INFINITY;
	for (size_t i = 0; i < n_elements; i++){
		if (!S[i] && D[i] < min){ // If not visited and weight < min
			min = D[i];
			pivot = i;
		}
	}
	return pivot;
}

DijkstraData_t graph_dijkstra(Graph *graph, void *source){
	DijkstraData_t dijkstra = {.D = NULL, .P = NULL, .n_elements = 0, .status = SUCCESS};
	if (!graph || !source){
		printerr_null_param();
		dijkstra.status = NULL_PARAMETER_ERROR;
		return dijkstra;
	}
	ptrdiff_t source_index = graph_indexof(graph, source);
	if (source_index < 0 || graph->n_elements == 0){
		dijkstra.status = source_index;
		return dijkstra;
	}

	graph_init_dijkstra(&dijkstra, graph, source_index);

	// Initialize the visited array
	uint8_t *S = calloc(graph->n_elements, sizeof(*S));
	assert(S);

	// Mark the start vertex as visited (because of the initialization)
	S[source_index] = 1;

	ptrdiff_t pivot = graph_get_pivot(S, dijkstra.D, graph->n_elements);
	while (pivot != -1){ // While there's still pivots
		for (size_t i = 0; i < graph->n_elements; i++){
			if (S[i]){ // If already visited continue
				continue;
			}
			// Calculate the cost to i through this pivot
			float w = dijkstra.D[pivot] + graph->weights[pivot][i];

			// If the cost is < that the actual cost AND it exists an edge between the pivot and i
			if (dijkstra.D[i] > w && graph->edges[pivot][i]){
				dijkstra.D[i] = w;
				dijkstra.P[i] = pivot;
			}
		}
		// Mark the pivot as visited and get the next one
		S[pivot] = 1;
		pivot = graph_get_pivot(S, dijkstra.D, graph->n_elements);
	}
	free(S); // Free the visited array
	return dijkstra;
}

void graph_print_dijkstra_data(void *output, DijkstraData_t data){
	FILE *_output = (FILE*) output;
	fprintf(_output, "[DIJKSTRA]\n");
	fprintf(_output, "i\tD\tP\n");
	for (size_t i = 0; i < data.n_elements; i++){
		if (data.P[i] >= 0){ // If the pivot exists, print it
			fprintf(_output, "%-3zu\t%.3f\t%zu\n", i, data.D[i], data.P[i]);
		} else { // Print a '-' to mark represent an unexisting pivot
 			fprintf(_output, "%-3zu\t%.3f\t-\n", i, data.D[i]);
		}
	}
}

void graph_free_dijkstra_data(DijkstraData_t *data){
	free(data->D);
	free(data->P);
	data->D = NULL;
	data->P = NULL;
}

///////////////////////////////////////////////////////////////////////////////

// FLOYD //////////////////////////////////////////////////////////////////////

/**
 * Initializes the FloydData_t struct
*/
static void graph_init_floyd(FloydData_t *floyd, Graph *graph){
	floyd->A = malloc(sizeof(*floyd->A) * graph->n_elements);
	floyd->P = malloc(sizeof(*floyd->P) * graph->n_elements);
	assert(floyd->A && floyd->P);

	for (size_t i = 0; i < graph->n_elements; i++){
		floyd->A[i] = malloc(sizeof(*floyd->A[i]) * graph->n_elements);
		floyd->P[i] = malloc(sizeof(*floyd->P[i]) * graph->n_elements);
		assert(floyd->A[i] && floyd->P[i]);

		for (size_t j = 0; j < graph->n_elements; j++){
			floyd->A[i][j] = graph->weights[i][j];
			floyd->P[i][j] = -1; // At the begining, all pivots are -1
		}
		floyd->A[i][i] = 0.0f; // The cost from an elements to itself is always 0
	}
	floyd->n_elements = graph->n_elements;
	floyd->status = SUCCESS;
}

FloydData_t graph_floyd(Graph *graph){
	FloydData_t floyd = {.A = NULL, .P = NULL, .n_elements = 0, .status = SUCCESS};
	if (!graph){
		printerr_null_param();
		floyd.status = NULL_PARAMETER_ERROR;
		return floyd;
	}
	graph_init_floyd(&floyd, graph);
	for (size_t pivot = 0; pivot < graph->n_elements; pivot++){
		for (size_t i = 0; i < graph->n_elements; i++){
			for (size_t j = 0; j < graph->n_elements; j++){
				float w = floyd.A[i][pivot] + floyd.A[pivot][j]; // Cost from i to j, going through the pivot
				if (floyd.A[i][j] > w){ // If it's better that the cost of going directly from i to j
					floyd.A[i][j] = w;
					floyd.P[i][j] = pivot;
				}
			}
		}
	}
	return floyd;
}

void graph_print_floyd_data(void *output, FloydData_t data){
	FILE *_output = (FILE*) output;
	fprintf(_output, "[FLOYD]\nD:\n");
	for (size_t i = 0; i < data.n_elements; i++){
		for (size_t j = 0; j < data.n_elements; j++){
			fprintf(_output, "%.2f\t", data.A[i][j]);
		}
		fprintf(_output, "\n");
	}
	fprintf(_output, "P:\n");
	for (size_t i = 0; i < data.n_elements; i++){
		for (size_t j = 0; j < data.n_elements; j++){
			if (data.P[i][j] >= 0){
				fprintf(_output, "%zu\t", data.P[i][j]);
			}else{
				fprintf(_output, "-\t");
			}
		}
		fprintf(_output, "\n");
	}
}

void graph_free_floyd_data(FloydData_t *data){
	for (size_t i = 0; i < data->n_elements; i++){
		free(data->A[i]);
		free(data->P[i]);
	}
	free(data->A);
	free(data->P);
	data->A = NULL;
	data->P = NULL;
}

///////////////////////////////////////////////////////////////////////////////

//// OTHER ALGORITHMS ////////////////////////////////////////////////////////

graph_degree graph_get_degree(Graph *graph, void *vertex){
	graph_degree degree = {0, 0, 0, NULL_PARAMETER_ERROR};
	if (!graph || !vertex){
		printerr_null_param();
		return degree;
	}
	ptrdiff_t index = graph_indexof(graph, vertex);
	if (index < 0){
		degree.status = index;
		return degree;
	}
	degree.status = SUCCESS;
	for (size_t i = 0; i < graph->n_elements; i++){
		if (graph->edges[i][index] == 1){
			degree.deg_in++;
		}
		if (graph->edges[index][i] == 1){
			degree.deg_out++;
		}
	}
	degree.deg = degree.deg_in + degree.deg_out;
	return degree;
}

bool graph_is_source_vertex(Graph *graph, void *vertex){
	graph_degree degree = graph_get_degree(graph, vertex);
	if (degree.status != SUCCESS){
		return false;
	}
	return degree.deg_in == 0 && degree.deg_out > 0;
}

bool graph_is_drain_vertex(Graph *graph, void *vertex){
	graph_degree degree = graph_get_degree(graph, vertex);
	if (degree.status != SUCCESS){
		return false;
	}
	return degree.deg_out == 0 && degree.deg_in > 0;
}

bool graph_is_isolated_vertex(Graph *graph, void *vertex){
	graph_degree degree = graph_get_degree(graph, vertex);
	if (degree.status != SUCCESS){
		return false;
	}
	return degree.deg == 0;
}

float graph_eccentricity(Graph *graph, void *vertex){
	if (!graph || !vertex){
		printerr_null_param();
		return NULL_PARAMETER_ERROR * 1.0f;
	}
	ptrdiff_t index = graph_indexof(graph, vertex);
	if (index < 0){
		return index * 1.0f;
	}

	FloydData_t floyd = graph_floyd(graph);
	float max = -1.0f;
	for (size_t i = 0; i < graph->n_elements; i++){
		if (floyd.A[i][index] > max){
			max = floyd.A[i][index];
		}
	}
	graph_free_floyd_data(&floyd);
	return max;
}

///////////////////////////////////////////////////////////////////////////////

////// Traverse ///////////////////////////////////////////////////////////////

static int traverse_df_rec(graph_traversal *data, size_t index, uint8_t *visited, Graph *graph){
	visited[index] = 1;
	void *dst = void_offset(data->elements, data->elements_size * graph->data_size);
	void *src = void_offset(graph->vertices, index * graph->data_size);
	memcpy(dst, src, graph->data_size);
	data->elements_size++;
	int s;
	for (size_t i = 0; i < graph->n_elements; i++){
		if (visited[i] == 0 && graph->edges[index][i] == 1){
			s = traverse_df_rec(data, i, visited, graph);
			if (s != SUCCESS){
				return s;
			}
		}
	}
	return SUCCESS;
}

graph_traversal graph_traverse_DF(Graph *graph, void *vertex){
	graph_traversal df = {NULL, 0, NULL_PARAMETER_ERROR};
	if (!graph){
		printerr_null_param();
		return df;
	}
	ptrdiff_t index = graph_indexof(graph, vertex);
	if (index < 0){
		df.status = index;
		return df;
	}
	df.status = SUCCESS;
	df.elements_size = 0;
	df.elements = calloc(graph->n_elements , graph->data_size);

	uint8_t *visited = calloc(graph->n_elements, sizeof(*visited));
	assert(visited);

	int s = traverse_df_rec(&df, index, visited, graph);
	if (s != SUCCESS){
		free(visited);
		free(df.elements);
		df.status = s;
		return df;
	}

	free(visited);
	return df;
}

graph_traversal graph_traverse_BF(Graph *graph, void *vertex){
	graph_traversal bf = {NULL, 0, NULL_PARAMETER_ERROR};
	if (!graph){
		printerr_null_param();
		return bf;
	}
	// Get index of the starting vertex
	ptrdiff_t index = graph_indexof(graph, vertex);
	if (index < 0){
		bf.status = index;
		return bf;
	}
	// Initialize result and temporary structures.
	bf.status = SUCCESS;
	bf.elements_size = 0;
	bf.elements = malloc(graph->n_elements * graph->data_size);
	uint8_t *visited = calloc(graph->n_elements, sizeof(*visited));
	size_t *queue = malloc(graph->n_elements * sizeof(*queue));
	assert(visited && queue);

	// Add starting element to queue and set it as visited
	size_t *start = queue;
	size_t *end = queue;
	*end++ = index;
	visited[index] = 1;

	void *src;
	void *dst = bf.elements;

	while (start < end){
		// Get next element from queue
		size_t piv = *start++;

		// Copy it into result array
		src = void_offset(graph->vertices, piv * graph->data_size);
		memcpy(dst, src, graph->data_size);
		dst = void_offset(dst, graph->data_size);
		bf.elements_size++;

		// Add all it's sons to queue
		for (size_t i = 0; i < graph->n_elements; ++i){
			if (!visited[i] && graph->edges[piv][i]){
				visited[i] = 1;
				*end++ = i;
			}
		}
	}
	free(queue);
	free(visited);
	return bf;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void free_contents(Graph *graph){
	if (graph->destructor){
		void *tmp = graph->vertices;
		for (size_t i = 0; i < graph->n_elements; i++){
			graph->destructor(tmp);
			tmp = void_offset(tmp, graph->data_size);
		}
	}
	free(graph->vertices);
	for (size_t i = 0; i < graph->max_elements; i++){
		free(graph->edges[i]);
		free(graph->weights[i]);
	}
	free(graph->edges);
	free(graph->weights);
}

int graph_free(Graph *graph){
	if (!graph){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	free_contents(graph);
	free(graph);
	return SUCCESS;
}

void graph_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		Graph *ptr = va_arg(arg, Graph*);
		graph_free(ptr);
	}
	va_end(arg);
}

Graph* graph_reset(Graph *graph){
	if (!graph){
		printerr_null_param();
		return NULL;
	}
	free_contents(graph);
	graph->n_elements = 0;
	graph->max_elements = 0;
	graph->edges = NULL;
	graph->vertices = NULL;
	graph->weights = NULL;
	expand_memory(graph, GRAPH_DEFAULT_SIZE);
	return graph;
}
