/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 11-01-2023
 * Copyright (C) 2022-2023 Saúl Valdelvira
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "graph.h"
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "../Util/index_t.h"

struct _Graph {
	size_t n_elements;
	size_t max_elements;
	size_t data_size;
	comparator_function_t compare;
	float **weights;
	int8_t **edges;
	void *nodes;
};

/**
 * Expands the graph number of elements. This means
 * 1) Allocates new spaces for nodes, weights and edges.
 * 2) Copies old values into the new, bigger spaces.
 * 3) Fills the new spaces with the default values (0 for edges and INFINITY for weights)
 *      In the case of edges, since we allocate them with calloc, the values are already 0 by default.
 * 4) Frees the old spaces
*/
static int expand_memory(size_t data_size, size_t old_size, size_t new_size, void **nodes_ptr, float ***weights_ptr, int8_t ***edges_ptr){
	// Allocate nodes
	void *nodes = malloc( new_size * data_size);
	if (!nodes){
		printerr_allocation(expand_memory);
		return ALLOCATION_ERROR;
	}

	// Allocate weights
	float **weights = malloc(new_size * sizeof(*weights));
	if (!weights){
		printerr_allocation(expand_memory);
		return ALLOCATION_ERROR;
	}

	// Allocate edges (only the columns)
	int8_t **edges = malloc(new_size * sizeof(*edges));
	if (!edges){
		printerr_allocation(expand_memory);
		return ALLOCATION_ERROR;
	}

	// Copy old node values in the range [0, oldSize) and free the old one
	void *tmp = memcpy(nodes, *nodes_ptr, old_size * data_size);
	if (!tmp){
		printerr_memory_op(expand_memory);
		return MEMORY_OP_ERROR;
	}
	free((*nodes_ptr));

	// Initialize the columns for edges and weights.
	// We divide the loop in two ranges [0 - oldSize) and [oldSize - newSize)
	// In the first one, we allocate the new memmory and copy the old values. 
	for (size_t i = 0; i < old_size; i++){
		weights[i] = malloc(new_size * sizeof(*weights[i]));
		if (!weights[i]){
			printerr_allocation(expand_memory);
			return ALLOCATION_ERROR;
		}

		tmp = memcpy(weights[i], (*weights_ptr)[i], sizeof(*weights[i])*old_size);
		if (!tmp){
			printerr_memory_op(expand_memory);
			return MEMORY_OP_ERROR;
		}

		edges[i] = calloc(new_size, sizeof(*edges[i]));
		if (!edges[i]){
			printerr_allocation(expand_memory);
			return ALLOCATION_ERROR;
		}

		tmp = memcpy(edges[i], (*edges_ptr)[i], sizeof(*edges[i])*old_size);
		if (!tmp){
			printerr_memory_op(expand_memory);
			return MEMORY_OP_ERROR;
		}

		// Set the new rows to INFINITY. Edges are already 0 because of the call to calloc
		for (size_t j = old_size; j < new_size; j++){
			weights[i][j] = INFINITY;
		}

		// Free the old pointers
		free((*weights_ptr)[i]);
		free((*edges_ptr)[i]);
	}

	// In the second one, we also allocate memory but we set the default values, since there are no old values to copy
	for (size_t i = old_size; i < new_size; i++){
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

	free((*weights_ptr));
	free((*edges_ptr));

	*nodes_ptr = nodes;
	*weights_ptr = weights;
	*edges_ptr = edges;
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

	void *nodes = NULL;
	float **weights = NULL;
	int8_t **edges = NULL;

	if (expand_memory(data_size, 0, n_elements,  &nodes, &weights, &edges) != SUCCESS){
		return NULL;
	}

	graph->n_elements = 0;
	graph->max_elements = n_elements;
	graph->compare = cmp;
	graph->weights = weights;
	graph->edges = edges;
	graph->nodes = nodes;
	graph->data_size = data_size;
	return graph;
}

int graph_add_node(Graph *graph, void *element){
	if (!graph || !element){
		printerr_null_param(graph_add_node);
		return NULL_PARAMETER_ERROR;
	}
	if (graph->n_elements == graph->max_elements){
		graph->max_elements *= 2;
		if (expand_memory(graph->data_size, graph->n_elements, graph->max_elements, &graph->nodes, &graph->weights, &graph->edges) != SUCCESS) {
			return ALLOCATION_ERROR;
		}
	}
	void *tmp = void_offset(graph->nodes, graph->n_elements * graph->data_size); // Position to copy value into
	tmp = memcpy(tmp, element, graph->data_size); // Perform the copy and store the reuslt
	if (!tmp){
		printerr_memory_op(graph_add_node);
		return MEMORY_OP_ERROR;
	}
	graph->n_elements++;
	return SUCCESS;
}

index_t graph_indexof(Graph *graph, void *element){
	for (size_t i = 0; i < graph->n_elements; i++){
		if((*graph->compare)(void_offset(graph->nodes, i * graph->data_size), element) == 0){
			return index_t(i,SUCCESS);
		}
	}
	return index_t(0,ELEMENT_NOT_FOUND_ERROR);
}

/**
 * Removes a node from the graph. This means
 * To do so it swaps the elements to delete with the last elements in the array of nodes.
 * It also has to swap the edges and weights values.
 * After decrementing the n_elements values, the old (now "removed") node becomes garbage memory to be overwritten in the next add
*/
int graph_remove_node(Graph *graph, void *element){
	if (!graph || !element){
		printerr_null_param(graph_remove_node);
		return NULL_PARAMETER_ERROR;
	}
	index_t index = graph_indexof(graph, element); // Get the index of the node
	if (index.status != SUCCESS){
		return index.status;
	}
	// Move latest node to this position (only if it is not already the last)
	if (index.value != graph->n_elements-1){
		// Set node at index to the last node in the array
		void *target = void_offset(graph->nodes, index.value * graph->data_size);
		void *source = void_offset(graph->nodes, (graph->n_elements-1) * graph->data_size);
		target = memmove(target, source, graph->data_size);
		if (!target){
			printerr_memory_op(graph_remove_node);
			return MEMORY_OP_ERROR;
		}

		// Swap the weights columns of the element to be removed and the last one 
		target = (void*) (graph->weights[index.value]);
		source = (void*) (graph->weights[graph->n_elements-1]);

		target = memmove(target, source, graph->max_elements * sizeof(*graph->weights[0]));
		if (!target){
			printerr_memory_op(graph_remove_node);
			return MEMORY_OP_ERROR;
		}

		// Swap the edges columns of the element to be removed and the last one 
		target = (void*) (graph->edges[index.value]);
		source = (void*) (graph->edges[graph->n_elements-1]);

		target = memmove(target, source, graph->max_elements * sizeof(*graph->edges[0]));
		if (!target){
			printerr_memory_op(graph_remove_node);
			return MEMORY_OP_ERROR;
		}

		// Swap rows of the element to be removed and the last one 
		for (size_t i = 0; i < graph->max_elements; i++){
			graph->edges[i][index.value] = graph->edges[i][graph->n_elements-1];
			graph->weights[i][index.value] = graph->weights[i][graph->n_elements-1];

			// We also have to set the edges and weights to default to guarantee that in the next add, those edges and weights will not hold garbage
			graph->edges[i][graph->n_elements-1] = 0;
			graph->edges[graph->n_elements-1][i] = 0;
			graph->weights[i][graph->n_elements-1] = INFINITY;
			graph->weights[graph->n_elements-1][i] = INFINITY;
		}
	// If the node to remove is the last, we still have to clear the values. The reason i didn't just put this loop outside the if-else
	// Statement is because if so, in the case we remove a non-last element (most) we should iteratre twice. This way, we have longer code. But
	// At runtime, the loop will only run once. (Hope I wrote this clear enough :p)
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

bool graph_exists_node(Graph *graph, void *element){
	if (!graph || !element){
		printerr_null_param(graph_exists);
		return false;
	}
	void *tmp;
	for (size_t i = 0; i < graph->n_elements; i++){
		tmp = void_offset(graph->nodes, graph->data_size * i);
		if((*graph->compare) (element, tmp) == 0){
			return true;
		}
	}
	return false;
}

size_t graph_n_elements(Graph *graph){
	if (!graph){
		printerr_null_param(graph_n_elements);
		return 0; // ??Also wrongggg
	}
	return graph->n_elements;
}

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
	// The source node's cheapest path is allways itself with weight 0
	dijkstra->D[source] = 0.0f;
	dijkstra->P[source].value = source;
	dijkstra->P[source].status = 1;

	dijkstra->n_elements = graph->n_elements;
	dijkstra->status = SUCCESS;
}

/**
 * Returns the next pivot for the algorithm. 
 * The next pivot is the node with the lowest cost that has not been visited yet
 * @param S an array of visited nodes. 1 if visited, 0 if not visited
 * @param D an array of weights
 * @param n_elements the number of elements in the arrays
*/
static index_t graph_get_pivot(u_int8_t *S, float *D, size_t n_elements){
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
	u_int8_t *S = calloc(graph->n_elements, sizeof(*S));
	if (!S) {
		printerr_allocation(graph_dijkstra);
		free(dijkstra.D);
		free(dijkstra.P);
		dijkstra.D = NULL;
		dijkstra.P = NULL;
		dijkstra.status = ALLOCATION_ERROR;
		return dijkstra;
	}

	// Mark the start node as visited (because of the initialization)
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

void graph_print_dijkstra_data(FILE *output, DijkstraData_t data) {
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

void graph_free_dijkstra_data(DijkstraData_t data){
	free(data.D);
	free(data.P);
}

static void free_contents(Graph *graph){
	free(graph->nodes);

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
	return 1;
}

Graph* graph_reset(Graph *graph){
	if (!graph){
		printerr_null_param(graph_reset);
		return NULL;
	}
	free_contents(graph);
	graph->n_elements = 0;
	graph->max_elements = GRAPH_DEFAULT_SIZE;
	graph->edges = NULL;
	graph->nodes = NULL;
	graph->weights = NULL;
	if (expand_memory(graph->data_size, 0, GRAPH_DEFAULT_SIZE,  &graph->nodes, &graph->weights, &graph->edges) != SUCCESS){
		return NULL;
	}
	return graph;
}