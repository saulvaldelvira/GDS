#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE
#include "../src/Graph/graph.h"

void dijkstra_test(void){
	Graph *g = graph_empty(sizeof(char) , compare_char);

	char a = 'A', b = 'B', c = 'C', d = 'D' , e = 'E';

	assert(graph_add_node(g, &a));
	assert(graph_add_node(g, &b));
	assert(graph_add_node(g, &c));
	assert(graph_add_node(g, &d));
	assert(graph_add_node(g, &e));

	assert(graph_add_edge(g, &a, &b, 1.0f));
	assert(graph_add_edge(g, &a, &d, 3.0f));
	assert(graph_add_edge(g, &a, &e, 10.0f));

	assert(graph_add_edge(g, &d, &e, 6.0f));
	assert(graph_add_edge(g, &d, &c, 2.0f));

	assert(graph_add_edge(g, &b, &c, 5.0f));

	assert(graph_add_edge(g, &c, &e, 1.0f));

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

	printf("\n****************************************************\n");
	printf("Here bellow should appear a NULL parameter error.\n");
	dijkstra = graph_dijkstra(NULL, NULL);
	assert(dijkstra.status == NULL_PARAMETER_ERROR);
	printf("****************************************************\n");

	graph_free(g);
	free(dijkstra.D);
	free(dijkstra.P);
}

void floyd_test(void){
	Graph *g = graph_empty(sizeof(char), compare_char);
	char a = 'A', b = 'B', c = 'C', d = 'D', e = 'E', f = 'F';
	assert(graph_add_node(g, &a));
	assert(graph_add_node(g, &b));
	assert(graph_add_node(g, &c));
	assert(graph_add_node(g, &d));
	assert(graph_add_node(g, &e));
	assert(graph_add_node(g, &f));

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

	graph_add_node(g, &a);
	graph_add_node(g, &b);
	graph_add_node(g, &c);
	graph_add_node(g, &d);

	graph_add_edge(g, &a, &b, 1.0f);
	graph_add_edge(g, &a, &c, 1.0f);
	graph_add_edge(g, &c, &d, 1.0f);
	graph_add_edge(g, &b, &d, 1.0f);

	assert(graph_is_drain_node(g, &d));
	assert(!graph_is_drain_node(g, &a));
	assert(!graph_is_drain_node(g, &c));

	assert(graph_is_source_node(g, &a));
	assert(!graph_is_source_node(g, &d));
	assert(!graph_is_source_node(g, &b));

	graph_free(g);
}

void traverse_df(void){
	Graph *graph = graph_empty(sizeof(char), compare_char);
	char a = 'A', b = 'B', c = 'C', d = 'D', 
		e = 'E', f = 'F', g = 'G', h = 'H', i = 'I', j = 'J';

	graph_add_node(graph, &a);
	graph_add_node(graph, &b);
	graph_add_node(graph, &c);
	graph_add_node(graph, &d);	
	graph_add_node(graph, &e);
	graph_add_node(graph, &f);
	graph_add_node(graph, &g);
	graph_add_node(graph, &h);
	graph_add_node(graph, &i);
	graph_add_node(graph, &j);

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

	traverse_df_data_t df = graph_traverse_DF(graph, &a);
	printf("DF: ");
	for (int i=0; i < 10; i++){
		void *tmp = void_offset(df.elements, i);
		printf("%c-", * (char*) tmp);
	}
	printf("\n");
	free(df.elements);
	graph_free(graph);
}

int main(){
	intptr_t n = 1200;
	printf("[Graph Test]\n");
	TIMESTAMP_START
	Graph *g = graph_empty(sizeof(int) ,compare_int);
	for(int i = 0; i < n; i++){
		assert(graph_add_node(g, &i));
		assert(graph_n_elements(g) == (size_t) i+1);
	}

	assert(graph_add_edge(g, cast_int(12), cast_int(15), 0.2f));
	assert(graph_exists_edge(g, cast_int(12), cast_int(15)));
	assert(graph_get_edge(g, cast_int(12), cast_int(15)) == 0.2f);

	assert(graph_add_edge(g, cast_int(2), cast_int(98), -0.2f));
	assert(graph_exists_edge(g, cast_int(2), cast_int(98)));
	assert(graph_get_edge(g, cast_int(2), cast_int(98)) == -0.2f);

	for(int i = 0; i < n; i++){
		assert(graph_exists_node(g, &i));
		assert(graph_remove_node(g, &i));
	}


	printf("DIJKSTRA...\n");
	dijkstra_test();

	printf("FLOYD...\n");
	floyd_test();

	printf("OTHER ALGORITHMS...\n");
	drain_source();
	traverse_df();

	TIMESTAMP_STOP
	graph_free(g);
	END_MSG(Graph)
	return 0;
}