#include "error.h"
#include "test.h"
#include "../include/graph.h"

void dijkstra_test(void){
	graph_t *g = graph_init(sizeof(char) , compare_char);

	char a = 'A', b = 'B', c = 'C', d = 'D' , e = 'E';

	assert(graph_add_vertex(g, &a));
	assert(graph_add_vertex(g, &b));
	assert(graph_add_vertex(g, &c));
	assert(graph_add_vertex(g, &d));
	assert(graph_add_vertex(g, &e));

	assert(graph_add_edge(g, &a, &b, 1.0f));
	assert(graph_add_edge(g, &a, &d, 3.0f));
	assert(graph_add_edge(g, &a, &e, 10.0f));

	assert(graph_add_edge(g, &d, &e, 6.0f));
	assert(graph_add_edge(g, &d, &c, 2.0f));

	assert(graph_add_edge(g, &b, &c, 5.0f));

	assert(graph_add_edge(g, &c, &e, 1.0f));

	/// config test
	graph_set_comparator(g, compare_equal);
	assert(graph_exists_vertex(g, &(int){-54654}));
	graph_set_comparator(g, compare_char);
	/////////////////

	DijkstraData_t dijkstra = graph_dijkstra(g, &a);

	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &b);

        assert(dijkstra.P[0] == -1);
	assert(dijkstra.P[1] != -1);
	assert(dijkstra.P[2] != -1);
	assert(dijkstra.P[3] == -1);
	assert(dijkstra.P[4] != -1);
	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &c);

	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &d);

	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &e);

	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &(char){'F'});
	assert(dijkstra.status == GDS_ELEMENT_NOT_FOUND_ERROR);
	graph_free_dijkstra_data(&dijkstra);

	graph_free(g);
	graph_free_dijkstra_data(&dijkstra);
}

void floyd_test(void){
	graph_t *g = graph_init(sizeof(char), compare_char);
	char a = 'A', b = 'B', c = 'C', d = 'D', e = 'E', f = 'F';
	assert(graph_add_vertex(g, &a));
	assert(graph_add_vertex(g, &b));
	assert(graph_add_vertex(g, &c));
	assert(graph_add_vertex(g, &d));
	assert(graph_add_vertex(g, &e));
	assert(graph_add_vertex(g, &f));

	assert(graph_add_edge(g, &a, &b, 3.0f));
	assert(graph_add_edge(g, &a, &c, 4.0f));
	assert(graph_add_edge(g, &a, &e, 8.0f));

	assert(graph_add_edge(g, &b, &e, 5.0f));

	assert(graph_add_edge(g, &c, &e, 3.0f));

	assert(graph_add_edge(g, &e, &f, 3.0f));
	assert(graph_add_edge(g, &e, &d, 7.0f));

	assert(graph_add_edge(g, &f, &d, 2.0f));

	FloydData_t floyd = graph_floyd(g);
	assert(floyd.status == GDS_SUCCESS);
	graph_free_floyd_data(&floyd);
	graph_free(g);
}

void drain_source(void){
	graph_t *g = graph_init(sizeof(char), compare_char);
	char a = 'A', b = 'B', c = 'C', d = 'D';

	graph_add_vertex(g, &a);
	graph_add_vertex(g, &b);
	graph_add_vertex(g, &c);
	graph_add_vertex(g, &d);

	graph_add_edge(g, &a, &b, 1.0f);
	graph_add_edge(g, &a, &c, 1.0f);
	graph_add_edge(g, &c, &d, 1.0f);
	graph_add_edge(g, &b, &d, 1.0f);

	assert(graph_is_drain_vertex(g, &d));
	assert(!graph_is_drain_vertex(g, &a));
	assert(!graph_is_drain_vertex(g, &c));

	assert(graph_is_source_vertex(g, &a));
	assert(!graph_is_source_vertex(g, &d));
	assert(!graph_is_source_vertex(g, &b));

	graph_free(g);
}

void traverse_bf(void){
	graph_t *graph = graph_init(sizeof(char), compare_char);
	char a = 'A', b = 'B', c = 'C', d = 'D', e= 'E', f ='F', g = 'G';
	graph_add_vertex(graph, &a);
	graph_add_vertex(graph, &b);
	graph_add_vertex(graph, &c);
	graph_add_vertex(graph, &d);
	graph_add_vertex(graph, &e);
	graph_add_vertex(graph, &f);
	graph_add_vertex(graph, &g);

	graph_add_edge(graph, &a, &b, 1.0f);
	graph_add_edge(graph, &a, &d, 1.0f);
	graph_add_edge(graph, &a, &e, 1.0f);

	graph_add_edge(graph, &e, &d, 1.0f);
	graph_add_edge(graph, &e, &c, 1.0f);
	graph_add_edge(graph, &d, &f, 1.0f);
	graph_add_edge(graph, &f, &g, 1.0f);
	graph_traversal_t result = graph_traverse_BF(graph, &a);
	assert(result.status == GDS_SUCCESS);

	char expected[] = {'A', 'B', 'D', 'E', 'F', 'C', 'G'};
	assert_array_char(result.elements, expected, result.elements_size);
	free(result.elements);
	graph_free(graph);
}

void traverse_df(void){
	graph_t *graph = graph_init(sizeof(char), compare_char);
	char a = 'A', b = 'B', c = 'C', d = 'D',
		e = 'E', f = 'F', g = 'G', h = 'H', i = 'I', j = 'J';

	graph_add_vertex(graph, &a);
	graph_add_vertex(graph, &b);
	graph_add_vertex(graph, &c);
	graph_add_vertex(graph, &d);
	graph_add_vertex(graph, &e);
	graph_add_vertex(graph, &f);
	graph_add_vertex(graph, &g);
	graph_add_vertex(graph, &h);
	graph_add_vertex(graph, &i);
	graph_add_vertex(graph, &j);

	graph_add_edge(graph, &a, &b, 1.0f);
	graph_add_edge(graph, &a, &h, 1.0f);

	graph_add_edge(graph, &b, &c, 1.0f);
	graph_add_edge(graph, &b, &d, 1.0f);
	graph_add_edge(graph, &b, &g, 1.0f);

	graph_add_edge(graph, &c, &e, 1.0f);
	graph_add_edge(graph, &c, &f, 1.0f);

	graph_add_edge(graph, &d, &i, 1.0f);

	graph_add_edge(graph, &e, &f, 1.0f);
	graph_add_edge(graph, &e, &d, 1.0f);

	graph_add_edge(graph, &g, &h, 1.0f);

	graph_add_edge(graph, &i, &j, 1.0f);
	graph_add_edge(graph, &i, &f, 1.0f);

#ifdef VERBOSE
	graph_traversal df = graph_traverse_DF(graph, &a);
	printf("DF: ");
	for (int n=0; n < 10; n++){
		void *tmp = void_offset(df.elements, n);
		printf("%c-", * (char*) tmp);
	}
	printf("\n");
	free(df.elements);
#endif
	graph_free(graph);
}

void eccentricity_test(void){
	graph_t *g = graph_init(sizeof(char), compare_char);
	char a = 'A', b = 'B', c = 'C', d = 'D', e = 'E';
	graph_add_vertex(g, &a);
	graph_add_vertex(g, &b);
	graph_add_vertex(g, &c);
	graph_add_vertex(g, &d);

	graph_add_edge(g, &b, &d, 1.0f);
	graph_add_edge(g, &d, &c, 1.0f);
	graph_add_edge(g, &c, &a, 1.0f);

	assert(graph_eccentricity(g, &a) == 3.0f);
	assert(graph_eccentricity(g, &b) == INFINITY);
	assert(graph_eccentricity(g, &c) == INFINITY);
	assert(graph_eccentricity(g, &d) == INFINITY);

	graph_add_edge(g, &a, &d, 2.0f);
	graph_add_edge(g, &c, &b, 5.0f);
	assert(graph_eccentricity(g, &b) == 8.0f);

	assert(graph_eccentricity(g, &(char){'J'}) == GDS_ELEMENT_NOT_FOUND_ERROR * 1.0f);

	graph_add_vertex(g, &e);
	graph_add_edge(g, &b, &e, 2.0f);
	assert(graph_eccentricity(g, &e) == 10.0f);

	graph_free(g);
}

void destructor_test(void){
	graph_t *g = graph_init(sizeof(int*), compare_lesser);
	graph_set_destructor(g, destroy_ptr);
	for (int i = 0; i < 1024; i++){
		int *ptr = malloc(sizeof(int));
		assert(graph_add_vertex(g, &ptr) == GDS_SUCCESS);
	}
	graph_free(g);
}

int main(void){
	intptr_t n = 1200;
	int tmp;
	test_start("graph.c");


	graph_t *g = graph_init(sizeof(int) ,compare_int);
	for(int i = 0; i < n; i++){
		assert(graph_add_vertex(g, &i));
		assert(graph_size(g) == (size_t) i+1);
		ptrdiff_t index = graph_indexof(g, &i);
		graph_vertex_at(g, index, &tmp);
		assert(tmp == i);
	}

	assert(graph_add_edge(g, &(int){12}, &(int){15}, 0.2f));
	assert(graph_exists_edge(g, &(int){12}, &(int){15}));
	assert(graph_get_edge(g, &(int){12}, &(int){15}) == 0.2f);

	assert(graph_add_edge(g, &(int){2}, &(int){98}, -0.2f));
	assert(graph_exists_edge(g, &(int){2}, &(int){98}));
	assert(graph_get_edge(g, &(int){2}, &(int){98}) == -0.2f);

	for(int i = 0; i < n; i++){
		assert(graph_exists_vertex(g, &i));
		assert(graph_remove_vertex(g, &i));
	}

	test_step("Dijkstra");
	dijkstra_test();
	test_ok();

	test_step("Floyd");
	floyd_test();
	test_ok();

	test_step("Traverse BF");
	traverse_bf();
	test_ok();

	drain_source();

	test_step("Traverse DF");
	traverse_df();
	test_ok();

	test_step("Eccentricity");
	eccentricity_test();
	test_ok();

	destructor_test();

	graph_clear(g);
	int vertices [] = {1, 2, 3, 4, 5};
	int sources [] = {1, 3, 4, 2, 5};
	int targets [] = {2, 1, 3, 2, 1};
	float weights[] = {1.2f, 1.0f, 23.4f, 1.0f, 1.0f};

	assert(graph_fill(g, vertices, sources, targets, weights, 5, 5));
	assert(5UL == graph_size(g));
	assert(1.0f == graph_get_edge(g, &sources[1], &targets[1]));
	assert(graph_remove_edges_array(g, sources, targets, 5));
	assert(!graph_exists_edge(g, &(int){3}, &(int){1}));
	assert(graph_remove_vertices_array(g, vertices, 5));
	assert(graph_isempty(g));

	graph_free(g);

	test_end("graph.c");
	return 0;
}
