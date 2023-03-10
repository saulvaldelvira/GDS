/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the GNU
 *  General Public License version 3, or any later version.
 *  See <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */
#include "graph.h"
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "../Util/index_t.h"

struct _Graph {
	size_t n_elements;
	size_t max_elements;
	size_t data_size;
	comparator_function_t compare;
	float **weights;
	int8_t **edges;
	void *vertices;
};

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
static int expand_memory(Graph *graph, size_t new_size){
	// Allocate vertices
	void *vertices = malloc(new_size * graph->data_size);

	// Allocate weights
	float **weights = malloc(new_size * sizeof(*weights));

	// Allocate edges
	int8_t **edges = malloc(new_size * sizeof(*edges));

	if (!edges || !weights || !edges){
		printerr_allocation(expand_memory);
		return ALLOCATION_ERROR;
	}

	// Copy old vertex values in the range [0, oldSize) and free the old one
	void *tmp = memcpy(vertices, graph->vertices, graph->max_elements * graph->data_size);
	if (!tmp){
		printerr_memory_op(expand_memory);
		return MEMORY_OP_ERROR;
	}
	free(graph->vertices);

	// Initialize the columns for edges and weights.
	// We divide the loop in two ranges [0 - oldSize) and [oldSize - newSize)
	// In the first one, we allocate the new memmory and copy the old values.
	for (size_t i = 0; i < graph->max_elements; i++){
		// Allocate weights[i] and copy old values
		weights[i] = malloc(new_size * sizeof(*weights[i]));
		if (!weights[i]){
			printerr_allocation(expand_memory);
			return ALLOCATION_ERROR;
		}

		tmp = memcpy(weights[i], graph->weights[i], sizeof(*weights[i])*graph->max_elements);
		if (!tmp){
			printerr_memory_op(expand_memory);
			return MEMORY_OP_ERROR;
		}

		// Allocate edges[i] and copy old values
		edges[i] = calloc(new_size, sizeof(*edges[i]));
		if (!edges[i]){
			printerr_allocation(expand_memory);
			return ALLOCATION_ERROR;
		}

		tmp = memcpy(edges[i], graph->edges[i], sizeof(*edges[i])*graph->max_elements);
		if (!tmp){
			printerr_memory_op(expand_memory);
			return MEMORY_OP_ERROR;
		}

		// Set the new rows to INFINITY. Edges are already 0 because of calloc.
		for (size_t j = graph->max_elements; j < new_size; j++){
			weights[i][j] = INFINITY;
		}

		// Free the old pointers
		free(graph->weights[i]);
		free(graph->edges[i]);
	}

	// In the second one [oldSize, newSize), we also allocate memory
	// but, since there are no old values to copy, we just set the default values
	for (size_t i = graph->max_elements; i < new_size; i++){
		weights[i] = malloc(new_size * sizeof(*weights[i]));
		if (!weights[i]){
			printerr_allocation(expand_memory);
			return ALLOCATION_ERROR;
		}

		edges[i] = calloc(new_size, sizeof(*edges[i]));
		if (!edges[i]){
			printerr_allocation(expand_memory);
			return ALLOCATION_ERROR;
		}

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
	return SUCCESS;
}

Graph* graph_empty(size_t data_size, comparator_function_t cmp){
	if (data_size <= 0){
		printerr_data_size(graph_empty);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(graph_empty);
		return NULL;
	}
	return graph_init(data_size, GRAPH_DEFAULT_SIZE, cmp);
}

Graph* graph_init(size_t data_size, size_t n_elements, comparator_function_t cmp){
	if (data_size <= 0){
		printerr_data_size(graph_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(graph_init);
		return NULL;
	}
	Graph *graph = malloc(sizeof(*graph));
	if (!graph){
		printerr_allocation(graph_init);
		return NULL;
	}

	graph->n_elements = 0;
	graph->max_elements = 0;
	graph->compare = cmp;
	graph->weights = NULL;
	graph->edges = NULL;
	graph->vertices = NULL;
	graph->data_size = data_size;

	// "Expanding" from 0 to n_elements turns into just initializing
	// weights, edges and vertices to their default size and state.
	if (expand_memory(graph, n_elements) != SUCCESS){
		return NULL;
	}

	return graph;
}

void graph_configure(Graph *graph, comparator_function_t cmp){
	if (!graph || !cmp){
		printerr_null_param(graph_configure);
		return;
	}
	graph->compare = cmp;
}

int graph_fill(Graph *graph, void *array_vertices, void *array_sources, void *array_targets, float *array_weights, size_t vertices_length, size_t edges_length){
	if (!graph || !array_vertices || !array_sources || !array_targets || !array_weights){
		printerr_null_param(graph_fill);
		return NULL_PARAMETER_ERROR;
	}
	int status = graph_add_vertices_array(graph, array_vertices, vertices_length);
	if (status != SUCCESS){
		return status;
	}
	status = graph_add_edges_array(graph, array_sources, array_targets, array_weights, edges_length);
	if (status != SUCCESS){
		return status;
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// vertices /////////////////////////////////////////////////////////////////////
int graph_add_vertex(Graph *graph, void *vertex){
	if (!graph || !vertex){
		printerr_null_param(graph_add_vertex);
		return NULL_PARAMETER_ERROR;
	}
	if (graph->n_elements == graph->max_elements){
		if (expand_memory(graph, graph->max_elements * 2) != SUCCESS){
			return ALLOCATION_ERROR;
		}
	}
	void *tmp = void_offset(graph->vertices, graph->n_elements * graph->data_size); // Position to copy value into
	tmp = memcpy(tmp, vertex, graph->data_size); // Perform the copy and store the reuslt
	if (!tmp){
		printerr_memory_op(graph_add_vertex);
		return MEMORY_OP_ERROR;
	}
	graph->n_elements++;
	return SUCCESS;
}

int graph_add_vertices_array(Graph *graph, void *array, size_t array_length){
	if (!graph || !array){
		printerr_null_param(graph_add_vertices_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * graph->data_size);
		status = graph_add_vertex(graph, tmp);
		if (status != SUCCESS){
			return status;
		}
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
		printerr_null_param(graph_remove_vertex);
		return NULL_PARAMETER_ERROR;
	}
	index_t index = graph_indexof(graph, vertex); // Get the index of the vertex
	if (index.status != SUCCESS){
		return index.status;
	}
	// Move latest vertex to this position (only if it is not already the last)
	if (index.value != graph->n_elements-1){
		// Set vertex at index to the last vertex in the array
		void *target = void_offset(graph->vertices, index.value * graph->data_size);
		void *source = void_offset(graph->vertices, (graph->n_elements-1) * graph->data_size);
		target = memmove(target, source, graph->data_size);
		if (!target){
			printerr_memory_op(graph_remove_vertex);
			return MEMORY_OP_ERROR;
		}

		// Swap the weights columns of the vertex to be removed and the last one
		target = (void*) (graph->weights[index.value]);
		source = (void*) (graph->weights[graph->n_elements-1]);

		target = memmove(target, source, graph->max_elements * sizeof(*graph->weights[0]));
		if (!target){
			printerr_memory_op(graph_remove_vertex);
			return MEMORY_OP_ERROR;
		}

		// Swap the edges columns of the vertex to be removed and the last one
		target = (void*) (graph->edges[index.value]);
		source = (void*) (graph->edges[graph->n_elements-1]);

		target = memmove(target, source, graph->max_elements * sizeof(*graph->edges[0]));
		if (!target){
			printerr_memory_op(graph_remove_vertex);
			return MEMORY_OP_ERROR;
		}

		// Swap rows of the vertex to be removed and the last one
		for (size_t i = 0; i < graph->max_elements; i++){
			graph->edges[i][index.value] = graph->edges[i][graph->n_elements-1];
			graph->weights[i][index.value] = graph->weights[i][graph->n_elements-1];

			// We also have to set the edges and weights to default to guarantee that in the next add, those edges and weights will not hold garbage
			graph->edges[i][graph->n_elements-1] = 0;
			graph->edges[graph->n_elements-1][i] = 0;
			graph->weights[i][graph->n_elements-1] = INFINITY;
			graph->weights[graph->n_elements-1][i] = INFINITY;
		}
	// If the vertex to remove is the last, we still have to clear the values.
	// The reason i didn't just put this loop outside the if-else
	// Statement is because if so, in the case we remove a non-last vertex (most) we should iteratre twice.
	// This way, we have longer code, but At runtime, the loop will only run once. (Hope I wrote this clear enough :p)
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
		printerr_null_param(graph_remove_vertices_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * graph->data_size);
		status = graph_remove_vertex(graph, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

bool graph_exists_vertex(Graph *graph, void *vertex){
	if (!graph || !vertex){
		printerr_null_param(graph_exists);
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
		printerr_null_param(graph_add_edge);
		return NULL_PARAMETER_ERROR;
	}
	index_t index_src = graph_indexof(graph, source);
	if (index_src.status != SUCCESS){
		return index_src.status;
	}
	index_t index_tar = graph_indexof(graph, target);
	if (index_tar.status != SUCCESS){
		return index_tar.status;
	}
	graph->edges[index_src.value][index_tar.value] = 1;
	graph->weights[index_src.value][index_tar.value] = weight;
	return SUCCESS;
}

int graph_add_edges_array(Graph *graph, void *array_sources, void *array_targets, float *array_weights, size_t arrays_length){
	if (!graph || !array_sources || !array_targets || !array_weights){
		printerr_null_param(graph_add_edges_array);
		return NULL_PARAMETER_ERROR;
	}
	void *src;
	void *trg;
	int status;
	for (size_t i = 0; i < arrays_length; i++){
		src = void_offset(array_sources, i * graph->data_size);
		trg = void_offset(array_targets, i * graph->data_size);
		status = graph_add_edge(graph, src, trg, array_weights[i]);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

int graph_remove_edge(Graph *graph, void *source, void *target){
	if (!graph || !source || !target){
		printerr_null_param(graph_remove_edge);
		return NULL_PARAMETER_ERROR;
	}
	index_t index_src = graph_indexof(graph, source);
	if (index_src.status != SUCCESS){
		return index_src.status;
	}
	index_t index_tar = graph_indexof(graph, target);
	if (index_tar.status != SUCCESS){
		return index_tar.status;
	}
	graph->edges[index_src.value][index_tar.value] = 0;
	graph->weights[index_src.value][index_tar.value] = INFINITY;
	return SUCCESS;
}

int graph_remove_edges_array(Graph *graph, void *array_sources, void *array_targets, size_t arrays_length){
	if (!graph || !array_sources || !array_targets){
		printerr_null_param(graph_remove_edges_array);
		return NULL_PARAMETER_ERROR;
	}
	void *src;
	void *trg;
	int status;
	for (size_t i = 0; i < arrays_length; i++){
		src = void_offset(array_sources, i * graph->data_size);
		trg = void_offset(array_targets, i * graph->data_size);
		status = graph_remove_edge(graph, src, trg);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

float graph_get_edge(Graph *graph, void *source, void *target){
	if (!graph || !source || !target){
		printerr_null_param(graph_get_edge);
		return NULL_PARAMETER_ERROR;
	}
	index_t index_src = graph_indexof(graph, source);
	if (index_src.status != SUCCESS){
		return index_src.status;
	}
	index_t index_tar = graph_indexof(graph, target);
	if (index_tar.status != SUCCESS){
		return index_tar.status;
	}
	return graph->weights[index_src.value][index_tar.value];
}

bool graph_exists_edge(Graph *graph, void *source, void *target){
	if (!graph || !source || !target){
		printerr_null_param(graph_exists_edge);
		return NULL_PARAMETER_ERROR;
	}
	index_t index_src = graph_indexof(graph, source);
	if (index_src.status != SUCCESS){
		return index_src.status;
	}
	index_t index_tar = graph_indexof(graph, target);
	if (index_tar.status != SUCCESS){
		return index_tar.status;
	}
	return graph->edges[index_src.value][index_tar.value];
}

///////////////////////////////////////////////////////////////////////////////

size_t graph_size(Graph *graph){
	if (!graph){
		printerr_null_param(graph_size);
		return 0; // ??Also wrongggg
	}
	return graph->n_elements;
}

bool graph_isempty(Graph *graph){
	if (!graph){
		printerr_null_param(graph_isempty);
		return false;
	}
	return graph->n_elements == 0;
}

index_t graph_indexof(Graph *graph, void *vertex){
	for (size_t i = 0; i < graph->n_elements; i++){
		if((*graph->compare)(void_offset(graph->vertices, i * graph->data_size), vertex) == 0){
			return index_t(i,SUCCESS);
		}
	}
	return index_t(0,ELEMENT_NOT_FOUND_ERROR);
}

/// DIJKSTRA //////////////////////////////////////////////////////////////////

/**
 * Initializes the DijkstraData_t structure.
*/
static void graph_init_dijkstra(DijkstraData_t *dijkstra, Graph *graph, size_t source){
	dijkstra->D = malloc(graph->n_elements * sizeof(*dijkstra->D));
	dijkstra->P = malloc(graph->n_elements * sizeof(*dijkstra->P));
	if (!dijkstra->D || !dijkstra->P){
		printerr_allocation(graph_dijkstra);
		free(dijkstra->D);
		free(dijkstra->P);
		dijkstra->status = ALLOCATION_ERROR;
		return;
	}

	for (size_t i = 0; i < graph->n_elements; i++){
		dijkstra->D[i] = graph->weights[source][i];
		if (graph->edges[source][i]){ // If there's an edge
			dijkstra->P[i].value = source; // Add the pivot
			dijkstra->P[i].status = 1; // Mark as valid
		} else{
			dijkstra->P[i].status = -1; // Mark as an index in wich there is no pivot
		}

	}
	// The source vertex's cheapest path is allways itself with weight 0
	dijkstra->D[source] = 0.0f;
	dijkstra->P[source].value = source;
	dijkstra->P[source].status = 1;

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
static index_t graph_get_pivot(uint8_t *S, float *D, size_t n_elements){
	index_t pivot = {.value = 0, .status=-1};
	float min = INFINITY;
	for (size_t i = 0; i < n_elements; i++){
		if (!S[i] && D[i] < min){ // If not visited and weight < min
			min = D[i];
			pivot.value = i;
			pivot.status = 1;
		}
	}
	return pivot;
}

DijkstraData_t graph_dijkstra(Graph *graph, void *source){
	DijkstraData_t dijkstra = {.D = NULL, .P = NULL, .n_elements = 0, .status = NULL_PARAMETER_ERROR};
	if (!graph || !source){
		printerr_null_param(graph_dijkstra);
		return dijkstra;
	}
	index_t source_index = graph_indexof(graph, source);
	if (source_index.status != SUCCESS || graph->n_elements == 0){
		dijkstra.status = source_index.status;
		return dijkstra;
	}

	graph_init_dijkstra(&dijkstra, graph, source_index.value);
	if (dijkstra.status != SUCCESS){
		return dijkstra;
	}

	// Initialize the visited array
	uint8_t *S = calloc(graph->n_elements, sizeof(*S));
	if (!S){
		printerr_allocation(graph_dijkstra);
		free(dijkstra.D);
		free(dijkstra.P);
		dijkstra.D = NULL;
		dijkstra.P = NULL;
		dijkstra.status = ALLOCATION_ERROR;
		return dijkstra;
	}

	// Mark the start vertex as visited (because of the initialization)
	S[source_index.value] = 1;

	index_t pivot = graph_get_pivot(S, dijkstra.D, graph->n_elements);
	while (pivot.status != -1){ // While there's still pivots
		size_t p = pivot.value;
		for (size_t i = 0; i < graph->n_elements; i++){
			if (S[i]){ // If already visited continue
				continue;
			}
			// Calculate the cost to i through this pivot
			float w = dijkstra.D[p] + graph->weights[p][i];

			// If the cost is < that the actual cost AND it exists an edge between the pivot and i
			if (dijkstra.D[i] > w && graph->edges[p][i]){
				dijkstra.D[i] = w;
				dijkstra.P[i].value = p;
				dijkstra.P[i].status = 1;
			}
		}
		// Mark the pivot as visited and get the next one
		S[p] = 1;
		pivot = graph_get_pivot(S, dijkstra.D, graph->n_elements);
	}

	free(S); // Free the visited array
	dijkstra.status = SUCCESS;
	return dijkstra;
}

void graph_print_dijkstra_data(FILE *output, DijkstraData_t data){
	fprintf(output, "[DIJKSTRA]\n");
	fprintf(output, "i\tD\tP\n");
	for (size_t i = 0; i < data.n_elements; i++){
		if (data.P[i].status == 1){ // If the pivot exists, print it
			fprintf(output, "%-3zu\t%.3f\t%zu\n", i, data.D[i], data.P[i].value);
		} else { // Print a '-' to mark represent an unexisting pivot
 			fprintf(output, "%-3zu\t%.3f\t-\n", i, data.D[i]);
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
	if (!floyd->A || !floyd->P){
		printerr_allocation(graph_init_floyd);
		floyd->status = ALLOCATION_ERROR;
		free(floyd->A);
		free(floyd->P);
		return;
	}

	for (size_t i = 0; i < graph->n_elements; i++){
		floyd->A[i] = malloc(sizeof(*floyd->A[i]) * graph->n_elements);
		floyd->P[i] = malloc(sizeof(*floyd->P[i]) * graph->n_elements);
		if (!floyd->A[i] || !floyd->P[i]){
			printerr_allocation(graph_init_floyd);
			floyd->n_elements = i+1; // Number of arrays already allocated
			graph_free_floyd_data(floyd); // Free everything before returning
			floyd->status = ALLOCATION_ERROR;
			return;
		}

		for (size_t j = 0; j < graph->n_elements; j++){
			floyd->A[i][j] = graph->weights[i][j];
			floyd->P[i][j].status = -1; // At the begining, all pivots are -1
		}
		floyd->A[i][i] = 0.0f; // The cost from an elements to itself is always 0
	}
	floyd->n_elements = graph->n_elements;
	floyd->status = SUCCESS;
}

FloydData_t graph_floyd(Graph *graph){
	FloydData_t floyd = {.A = NULL, .P = NULL, .n_elements = 0, .status = NULL_PARAMETER_ERROR};
	if (!graph){
		printerr_null_param(graph_floyd);
		return floyd;
	}
	graph_init_floyd(&floyd, graph);
	if (floyd.status != SUCCESS){
		return floyd;
	}
	for (size_t pivot = 0; pivot < graph->n_elements; pivot++){
		for (size_t i = 0; i < graph->n_elements; i++){
			for (size_t j = 0; j < graph->n_elements; j++){
				float w = floyd.A[i][pivot] + floyd.A[pivot][j]; // Cost from i to j, going through the pivot
				if (floyd.A[i][j] > w){ // If it's better that the cost of going directly from i to j
					floyd.A[i][j] = w;
					floyd.P[i][j].value = pivot;
					floyd.P[i][j].status = 1;
				}
			}
		}
	}
	return floyd;

}

void graph_print_floyd_data(FILE *output, FloydData_t data){
	fprintf(output, "[FLOYD]\nD:\n");
	for (size_t i = 0; i < data.n_elements; i++){
		for (size_t j = 0; j < data.n_elements; j++){
			fprintf(output, "%.2f\t", data.A[i][j]);
		}
		fprintf(output, "\n");
	}
	fprintf(output, "P:\n");
	for (size_t i = 0; i < data.n_elements; i++){
		for (size_t j = 0; j < data.n_elements; j++){
			if (data.P[i][j].status == 1){
				fprintf(output, "%zu\t", data.P[i][j].value);
			}else{
				fprintf(output, "-\t");
			}
		}
		fprintf(output, "\n");
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

vertexDegree_t graph_get_degree(Graph *graph, void *vertex){
	vertexDegree_t degree = {0, 0, 0, NULL_PARAMETER_ERROR};
	if (!graph || !vertex){
		printerr_null_param(graph_get_degree);
		return degree;
	}
	index_t index = graph_indexof(graph, vertex);
	if (index.status != SUCCESS){
		degree.status = index.status;
		return degree;
	}
	degree.status = SUCCESS;
	for (size_t i = 0; i < graph->n_elements; i++){
		if (graph->edges[i][index.value] == 1){
			degree.deg_in++;
		}
		if (graph->edges[index.value][i] == 1){
			degree.deg_out++;
		}
	}
	degree.deg = degree.deg_in + degree.deg_out;
	return degree;
}

bool graph_is_source_vertex(Graph *graph, void *vertex){
	vertexDegree_t degree = graph_get_degree(graph, vertex);
	if (degree.status != SUCCESS){
		return false;
	}
	return degree.deg_in == 0 && degree.deg_out > 0;
}

bool graph_is_drain_vertex(Graph *graph, void *vertex){
	vertexDegree_t degree = graph_get_degree(graph, vertex);
	if (degree.status != SUCCESS){
		return false;
	}
	return degree.deg_out == 0 && degree.deg_in > 0;
}

bool graph_is_isolated_vertex(Graph *graph, void *vertex){
	vertexDegree_t degree = graph_get_degree(graph, vertex);
	if (degree.status != SUCCESS){
		return false;
	}
	return degree.deg == 0;
}

float graph_eccentricity(Graph *graph, void *vertex){
	if (!graph || !vertex){
		printerr_null_param(graph_eccentricity);
		return NULL_PARAMETER_ERROR * 1.0f;
	}
	index_t index = graph_indexof(graph, vertex);
	if (index.status != SUCCESS){
		return index.status * 1.0f;
	}

	FloydData_t floyd = graph_floyd(graph);
	float max = -1.0f;
	for (size_t i = 0; i < graph->n_elements; i++){
		if (floyd.A[i][index.value] > max){
			max = floyd.A[i][index.value];
		}
	}
	graph_free_floyd_data(&floyd);
	return max;
}

///////////////////////////////////////////////////////////////////////////////

////// Deep First Traverse ////////////////////////////////////////////////////
static int traverse_df_rec(traverse_df_data_t *data, size_t index, uint8_t *visited, Graph *graph){
	visited[index] = 1;
	void *dst = void_offset(data->elements, data->elements_size * graph->data_size);
	void *src = void_offset(graph->vertices, index * graph->data_size);
	dst = memcpy(dst, src, graph->data_size);
	if (!dst){
		printerr_memory_op(traverse_df_rec);
		free(data->elements);
		return MEMORY_OP_ERROR;
	}
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

traverse_df_data_t graph_traverse_DF(Graph *graph, void *vertex){
	traverse_df_data_t df = {NULL, 0, NULL_PARAMETER_ERROR};
	if (!graph){
		printerr_null_param(graph_traverse_DF);
		return df;
	}
	index_t index = graph_indexof(graph, vertex);
	if (index.status != SUCCESS){
		df.status = index.status;
		return df;
	}
	df.status = SUCCESS;
	df.elements_size = 0;
	df.elements = calloc(graph->n_elements , graph->data_size);

	uint8_t *visited = calloc(graph->n_elements, sizeof(*visited));
	if (!df.elements || !visited){
		printerr_allocation(graph_traverse_DF);
		df.status = ALLOCATION_ERROR;
		return df;
	}

	int s = traverse_df_rec(&df, index.value, visited, graph);
	if (s != SUCCESS){
		free(visited);
		free(df.elements);
		df.status = s;
		return df;
	}

	free(visited);
	return df;
}
///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void free_contents(Graph *graph){
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
		printerr_null_param(graph_free);
		return NULL_PARAMETER_ERROR;
	}
	free_contents(graph);
	free(graph);
	return SUCCESS;
}

Graph* graph_reset(Graph *graph){
	if (!graph){
		printerr_null_param(graph_reset);
		return NULL;
	}
	free_contents(graph);
	graph->n_elements = 0;
	graph->max_elements = 0;
	graph->edges = NULL;
	graph->vertices = NULL;
	graph->weights = NULL;
	if (expand_memory(graph, GRAPH_DEFAULT_SIZE) != SUCCESS){
		return NULL;
	}
	return graph;
}
