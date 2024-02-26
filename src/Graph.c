/*
 * Graph.c - Graph implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "Graph.h"
#include "./util/error.h"
#include "./util/definitions.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

struct Graph {
        size_t n_elements;      ///< Number of elements in the Graph
        size_t max_elements;    ///< Current capacity of the graph
        size_t data_size;       ///< Size (in bytes) of the data type
        comparator_function_t compare;          ///< Comparator function pointer
        destructor_function_t destructor;       ///< Destructor function pointer
        float **weights;        ///< Weights matrix
        int8_t **edges;         ///< Edges matrix
        void *vertices;         ///< Vertices matrix
};

#define GRAPH_DEFAULT_SIZE 8

/// CONSTRUCTORS //////////////////////////////////////////////////////////////

static int expand_memory(Graph *graph, size_t new_size){
        void *vertices = malloc(new_size * graph->data_size);
        float **weights = malloc(new_size * sizeof(*weights));
        int8_t **edges = malloc(new_size * sizeof(*edges));
        if (!vertices || !weights || !edges){
                free(vertices);
                free(weights);
                free(edges);
                return ERROR;
        }

        if (graph->vertices)
                memcpy(vertices, graph->vertices, graph->max_elements * graph->data_size);

        for (size_t i = 0; i < new_size; i++){
                weights[i] = malloc(new_size * sizeof(*weights[i]));
                edges[i] = malloc(new_size * sizeof(*edges[i]));

                if (!weights[i] || !edges[i]){
                        for (size_t j = 0; j <= i; j++){
                                free(weights[j]);
                                free(edges[j]);
                        }
                        free(vertices);
                        free(weights);
                        free(edges);
                        return ERROR;
                }

                if (i < graph->max_elements){
                        memcpy(weights[i], graph->weights[i], sizeof(*weights[i])*graph->max_elements);
                        memcpy(edges[i], graph->edges[i], sizeof(*edges[i])*graph->max_elements);
                }
        }

        // Free old pointers
        for (size_t i = 0; i < graph->max_elements; i++){
                free(graph->weights[i]);
                free(graph->edges[i]);
        }
        free(graph->vertices);
        free(graph->weights);
        free(graph->edges);

        graph->vertices = vertices;
        graph->weights = weights;
        graph->edges = edges;
        graph->max_elements = new_size;
        return SUCCESS;
}

Graph* graph_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        Graph *graph = malloc(sizeof(*graph));
        if (!graph) return NULL;
        graph->n_elements = 0;
        graph->max_elements = 0;
        graph->compare = cmp;
        graph->destructor = NULL;
        graph->weights = NULL;
        graph->edges = NULL;
        graph->vertices = NULL;
        graph->data_size = data_size;
        if (expand_memory(graph, GRAPH_DEFAULT_SIZE) == ERROR){
                free(graph);
                return NULL;
        }
        return graph;
}

void graph_set_comparator(Graph *graph, comparator_function_t cmp){
        if (graph && cmp)
                graph->compare = cmp;
}

void graph_set_destructor(Graph *graph, destructor_function_t destructor){
        if (graph)
                graph->destructor = destructor;
}

int graph_fill(Graph *graph, void *array_vertices,
               void *array_sources, void *array_targets, float *array_weights,
               size_t vertices_length, size_t edges_length
){
        assert(graph && array_vertices && array_sources && array_targets && array_weights);
        graph_add_vertices_array(graph, array_vertices, vertices_length);
        graph_add_edges_array(graph, array_sources, array_targets, array_weights, edges_length);
        return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// vertices /////////////////////////////////////////////////////////////////////
int graph_add_vertex(Graph *graph, void *vertex){
        assert(graph && vertex);
        if (graph->n_elements == graph->max_elements){
                if (expand_memory(graph, graph->max_elements * 2) == ERROR)
                        return ERROR;
        }
        void *tmp = void_offset(graph->vertices, graph->n_elements * graph->data_size);
        memcpy(tmp, vertex, graph->data_size);
        /* Clear the edges and weights arrays, since they could
           contain garbage from a previously removed vertex */
        for (size_t i = 0; i < graph->max_elements; i++){
                graph->edges[i][graph->n_elements] = 0;
                graph->edges[graph->n_elements][i] = 0;
                graph->weights[i][graph->n_elements] = INFINITY;
                graph->weights[graph->n_elements][i] = INFINITY;
        }
        graph->n_elements++;
        return SUCCESS;
}

int graph_add_vertices_array(Graph *graph, void *array, size_t array_length){
        assert(graph && array);
        while (array_length-- > 0){
                int status = graph_add_vertex(graph, array);
                if (status != SUCCESS){
                        return status;
                }
                array = void_offset(array, graph->data_size);
        }
        return SUCCESS;
}

int graph_remove_vertex(Graph *graph, void *vertex){
        assert(graph && vertex);
        ptrdiff_t index = graph_indexof(graph, vertex);
        if (index < 0)
                return index;
        graph->n_elements--;
        if ((size_t)index == graph->n_elements)
                return SUCCESS;
        void *target = void_offset(graph->vertices, index * graph->data_size);
        void *source = void_offset(graph->vertices, graph->n_elements * graph->data_size);
        memmove(target, source, graph->data_size);

        target = graph->weights[index];
        source = graph->weights[graph->n_elements];
        memmove(target, source, graph->max_elements * sizeof(*graph->weights[0]));

        target = graph->edges[index];
        source = graph->edges[graph->n_elements];
        memmove(target, source, graph->max_elements * sizeof(*graph->edges[0]));

        for (size_t i = 0; i < graph->max_elements; i++){
                graph->edges[i][index] = graph->edges[i][graph->n_elements];
                graph->weights[i][index] = graph->weights[i][graph->n_elements];
        }
        return SUCCESS;
}

int graph_remove_vertices_array(Graph *graph, void *array, size_t array_length){
        assert(graph && array);
        while (array_length-- > 0){
                graph_remove_vertex(graph, array);
                array = void_offset(array, graph->data_size);
        }
        return SUCCESS;
}

bool graph_exists_vertex(Graph *graph, void *vertex){
        assert(graph && vertex);
        for (size_t i = 0; i < graph->n_elements; i++){
                void *tmp = void_offset(graph->vertices, graph->data_size * i);
                if((*graph->compare) (vertex, tmp) == 0){
                        return true;
                }
        }
        return false;
}

///////////////////////////////////////////////////////////////////////////////

///// EDGES ///////////////////////////////////////////////////////////////////

int graph_add_edge(Graph *graph, void *source, void *target, float weight){
        assert(graph && source && target);
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
        assert(graph && array_sources && array_targets && array_weights);
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
        assert(graph && source && target);
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
        assert(graph && array_sources && array_targets);
        while (arrays_length-- > 0){
                graph_remove_edge(graph, array_sources, array_targets);
                array_sources = void_offset(array_sources, graph->data_size);
                array_targets = void_offset(array_targets, graph->data_size);
        }
        return SUCCESS;
}

void* graph_vertex_at(Graph *graph, ptrdiff_t index, void *dest){
        assert(graph && dest);
        if (index < 0 || (size_t)index >= graph->n_elements)
                return NULL;
        void *src = void_offset(graph->vertices, index * graph->data_size);
        return memcpy(dest, src, graph->data_size);
}

float graph_get_edge(Graph *graph, void *source, void *target){
        assert(graph && source && target);
        ptrdiff_t index_src = graph_indexof(graph, source);
        if (index_src < 0)
                return index_src;
        ptrdiff_t index_dst = graph_indexof(graph, target);
        if (index_dst < 0)
                return index_dst;
        return graph->weights[index_src][index_dst];
}

bool graph_exists_edge(Graph *graph, void *source, void *target){
        assert(graph && source && target);
        ptrdiff_t index_src = graph_indexof(graph, source);
        ptrdiff_t index_dst = graph_indexof(graph, target);
        if (index_src < 0 || index_dst < 0)
                return false;
        return graph->edges[index_src][index_dst];
}

///////////////////////////////////////////////////////////////////////////////

size_t graph_size(Graph *graph){
        return graph ? graph->n_elements : 0;
}

bool graph_isempty(Graph *graph){
        return graph ? graph->n_elements == 0 : true;
}

ptrdiff_t graph_indexof(Graph *graph, void *vertex){
        void *tmp = graph->vertices;
        for (size_t i = 0; i < graph->n_elements; i++){
                if(graph->compare(tmp, vertex) == 0)
                        return i;
                tmp = void_offset(tmp, graph->data_size);
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
        if (!dijkstra->D || !dijkstra->P){
                free(dijkstra->D);
                free(dijkstra->P);
                dijkstra->status = ERROR;
                return;
        }

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
                if (!S[i] && D[i] < min){
                        min = D[i];
                        pivot = i;
                }
        }
        return pivot;
}

DijkstraData_t graph_dijkstra(Graph *graph, void *source){
        assert(graph && source);
        DijkstraData_t dijkstra = {.D = NULL, .P = NULL, .n_elements = 0, .status = SUCCESS};
        ptrdiff_t source_index = graph_indexof(graph, source);
        if (source_index < 0){
                dijkstra.status = source_index;
                return dijkstra;
        }

        graph_init_dijkstra(&dijkstra, graph, source_index);
        if (dijkstra.status != SUCCESS) return dijkstra;

        uint8_t *S = calloc(graph->n_elements, sizeof(*S));
        if (!S){
                free(dijkstra.D);
                free(dijkstra.P);
                dijkstra.status = ERROR;
                return dijkstra;
        }

        if ((size_t)source_index >= graph->n_elements){
                dijkstra.status = INDEX_BOUNDS_ERROR;
                free(dijkstra.D);
                free(dijkstra.P);
                free(S);
                return dijkstra;
        }

        S[source_index] = 1;

        ptrdiff_t pivot = graph_get_pivot(S, dijkstra.D, graph->n_elements);
        while (pivot != -1){
                for (size_t i = 0; i < graph->n_elements; i++){
                        if (S[i]) continue;
                        float w = dijkstra.D[pivot] + graph->weights[pivot][i];
                        if (dijkstra.D[i] > w && graph->edges[pivot][i]){
                                dijkstra.D[i] = w;
                                dijkstra.P[i] = pivot;
                        }
                }
                S[pivot] = 1;
                pivot = graph_get_pivot(S, dijkstra.D, graph->n_elements);
        }
        free(S);
        return dijkstra;
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
                free(floyd->A);
                free(floyd->P);
                floyd->status = ERROR;
                return;
        }

        for (size_t i = 0; i < graph->n_elements; i++){
                floyd->A[i] = malloc(sizeof(*floyd->A[i]) * graph->n_elements);
                floyd->P[i] = malloc(sizeof(*floyd->P[i]) * graph->n_elements);
                if (!floyd->A[i] || !floyd->P[i]){
                        for (size_t j = 0; j <= i; j++){
                                free(floyd->A[j]);
                                free(floyd->P[j]);
                        }
                        free(floyd->A);
                        free(floyd->P);
                        floyd->status = ERROR;
                        return;
                }

                for (size_t j = 0; j < graph->n_elements; j++){
                        floyd->A[i][j] = graph->weights[i][j];
                        floyd->P[i][j] = -1;
                }
                floyd->A[i][i] = 0.0f;
        }
        floyd->n_elements = graph->n_elements;
        floyd->status = SUCCESS;
}

FloydData_t graph_floyd(Graph *graph){
        assert(graph);
        FloydData_t floyd = {.A = NULL, .P = NULL, .n_elements = 0, .status = SUCCESS};
        graph_init_floyd(&floyd, graph);
        if (floyd.status != SUCCESS) return floyd;
        for (size_t pivot = 0; pivot < graph->n_elements; pivot++){
                for (size_t i = 0; i < graph->n_elements; i++){
                        for (size_t j = 0; j < graph->n_elements; j++){
                                float w = floyd.A[i][pivot] + floyd.A[pivot][j];
                                if (floyd.A[i][j] > w){
                                        floyd.A[i][j] = w;
                                        floyd.P[i][j] = pivot;
                                }
                        }
                }
        }
        return floyd;
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
        assert(graph && vertex);
        graph_degree degree = {0, 0, 0, SUCCESS};
        ptrdiff_t index = graph_indexof(graph, vertex);
        if (index < 0){
                degree.status = index;
                return degree;
        }
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
        assert(graph && vertex);
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
        assert(graph);
        graph_traversal df = {NULL, 0, SUCCESS};
        ptrdiff_t index = graph_indexof(graph, vertex);
        if (index < 0){
                df.status = index;
                return df;
        }
        df.elements_size = 0;
        df.elements = calloc(graph->n_elements , graph->data_size);

        uint8_t *visited = calloc(graph->n_elements, sizeof(*visited));
        if (!visited){
                free(df.elements);
                df.elements = NULL;
                df.status = ERROR;
                return df;
        }

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
        assert(graph);
        graph_traversal bf = {NULL, 0, SUCCESS};
        // Get index of the starting vertex
        ptrdiff_t index = graph_indexof(graph, vertex);
        if (index < 0){
                bf.status = index;
                return bf;
        }
        // Initialize result and temporary structures.
        bf.elements_size = 0;
        bf.elements = malloc(graph->n_elements * graph->data_size);
        uint8_t *visited = calloc(graph->n_elements, sizeof(*visited));
        size_t *queue = malloc(graph->n_elements * sizeof(*queue));
        if (!bf.elements || !visited || !queue){
                free(queue);
                free(visited);
                free(bf.elements);
                bf.elements = NULL;
                bf.status = ERROR;
                return bf;
        }

        // Add starting element to queue and set it as visited
        size_t *start = queue;
        size_t *end = queue;
        *end++ = index;
        visited[index] = 1;

        void *dst = bf.elements;

        while (start < end){
                // Get next element from queue
                size_t piv = *start++;

                // Copy it into result array
                void *src = void_offset(graph->vertices, piv * graph->data_size);
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

void graph_free(Graph *graph){
        if (graph){
                free_contents(graph);
                free(graph);
        }
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

void graph_clear(Graph *graph){
        if (!graph)
                return;
        free_contents(graph);
        graph->n_elements = 0;
        graph->max_elements = 0;
        graph->edges = NULL;
        graph->vertices = NULL;
        graph->weights = NULL;
        expand_memory(graph, GRAPH_DEFAULT_SIZE);
}
