#define TIMESTAMP_ENABLE
#include "test.h"
#undef TIMESTAMP_ENABLE
#include "../src/Graph/graph.h"

void dijkstra_test(void){
	Graph *g = graph_empty(sizeof(char) , compare_char);

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
	graph_configure(g, compare_allways_true);
	assert(graph_exists_vertex(g, cast_int(-54654)));
	graph_configure(g, compare_char);
	/////////////////

	DijkstraData_t dijkstra = graph_dijkstra(g, &a);
	graph_print_dijkstra_data(stdout, dijkstra);
	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &b);
	graph_print_dijkstra_data(stdout, dijkstra);
	assert(dijkstra.P[0].status != 1);
	assert(dijkstra.P[1].status == 1);
	assert(dijkstra.P[2].status == 1);
	assert(dijkstra.P[3].status != 1);
	assert(dijkstra.P[4].status == 1);
	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &c);
	graph_print_dijkstra_data(stdout, dijkstra);
	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &d);
	graph_print_dijkstra_data(stdout, dijkstra);
	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, &e);
	graph_print_dijkstra_data(stdout, dijkstra);
	graph_free_dijkstra_data(&dijkstra);

	dijkstra = graph_dijkstra(g, cast_char('F'));
	assert(dijkstra.status == ELEMENT_NOT_FOUND_ERROR);
	graph_free_dijkstra_data(&dijkstra);

#ifndef QUIET
	printf("\n****************************************************\n");
	printf("Here bellow should appear a NULL parameter error.\n");
	dijkstra = graph_dijkstra(NULL, NULL);
	assert(dijkstra.status == NULL_PARAMETER_ERROR);
	printf("****************************************************\n");
#endif

	graph_free(g);
	graph_free_dijkstra_data(&dijkstra);
}
	
void floyd_test(void){
	Graph *g = graph_empty(sizeof(char), compare_char);
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
	assert(floyd.status == SUCCESS);
	graph_print_floyd_data(stdout, floyd);
	graph_free_floyd_data(&floyd);
	graph_free(g);
}

void drain_source(void){
	Graph *g = graph_empty(sizeof(char), compare_char);
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

void traverse_df(void){
	Graph *graph = graph_empty(sizeof(char), compare_char);
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

#ifndef QUIET
	traverse_df_data_t df = graph_traverse_DF(graph, &a);
	printf("DF: ");
	for (int i=0; i < 10; i++){
		void *tmp = void_offset(df.elements, i);
		printf("%c-", * (char*) tmp);
	}
	printf("\n");
	free(df.elements);
#endif
	graph_free(graph);
}

void eccentricity_test(void){
	Graph *g = graph_empty(sizeof(char), compare_char);
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

#ifndef QUIET
	printf("\n********************************************************\n");
	printf("Here bellow should appear a NULL parameter error.\n");
	assert(graph_eccentricity(NULL, NULL) == NULL_PARAMETER_ERROR * 1.0f);
	printf("********************************************************\n");
#endif
	assert(graph_eccentricity(g, cast_char('J')) == ELEMENT_NOT_FOUND_ERROR * 1.0f);

	graph_add_vertex(g, &e);
	graph_add_edge(g, &b, &e, 2.0f);
	assert(graph_eccentricity(g, &e) == 10.0f);

	graph_free(g);
}

int main(){
	intptr_t n = 1200;
	printf("[Graph Test]\n");
#ifdef QUIET
	printf("...\n");
#endif

	TIMESTAMP_START
	Graph *g = graph_empty(sizeof(int) ,compare_int);
	for(int i = 0; i < n; i++){
		assert(graph_add_vertex(g, &i));
		assert(graph_size(g) == (size_t) i+1);
	}

	assert(graph_add_edge(g, cast_int(12), cast_int(15), 0.2f));
	assert(graph_exists_edge(g, cast_int(12), cast_int(15)));
	assert(graph_get_edge(g, cast_int(12), cast_int(15)) == 0.2f);

	assert(graph_add_edge(g, cast_int(2), cast_int(98), -0.2f));
	assert(graph_exists_edge(g, cast_int(2), cast_int(98)));
	assert(graph_get_edge(g, cast_int(2), cast_int(98)) == -0.2f);

	for(int i = 0; i < n; i++){
		assert(graph_exists_vertex(g, &i));
		assert(graph_remove_vertex(g, &i));
	}

	

#ifndef QUIET
	printf("Dijkstra...\n");
	dijkstra_test();


	printf("\nFloyd...\n");
	floyd_test();
#endif

	

	drain_source();


	LOG(printf("\nTraverse DF...\n"));
	traverse_df();

	

	LOG(printf("\nEccentricity...\n"));
	eccentricity_test();

	g = graph_reset(g);
	int vertices [] = {1, 2, 3, 4, 5};
	int sources [] = {1, 3, 4, 2, 5};
	int targets [] = {2, 1, 3, 2, 1};
	float weights[] = {1.2f, 1.0f, 23.4f, 1.0f, 1.0f};

	assert(graph_fill(g, vertices, sources, targets, weights, 5, 5));
	assert(5UL == graph_size(g));
	assert(1.0f == graph_get_edge(g, &sources[1], &targets[1]));
	assert(graph_remove_edges_array(g, sources, targets, 5));
	assert(!graph_exists_edge(g, cast_int(3), cast_int(1)));
	assert(graph_remove_vertices_array(g, vertices, 5));
	assert(graph_isempty(g));

	graph_free(g);
	TIMESTAMP_STOP
	END_MSG(Graph)
	return 0;
}