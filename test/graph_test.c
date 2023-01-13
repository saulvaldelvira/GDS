#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE
#include "../src/Graph/graph.h"

void dijkstra_test(){
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

	dijkstra = graph_dijkstra(g, &b);
	graph_print_dijkstra_data(stdout, dijkstra);
	assert(dijkstra.P[0].status != 1);
	assert(dijkstra.P[1].status == 1);
	assert(dijkstra.P[2].status == 1);
	assert(dijkstra.P[3].status != 1);
	assert(dijkstra.P[4].status == 1);

	dijkstra = graph_dijkstra(g, &c);
	graph_print_dijkstra_data(stdout, dijkstra);

	dijkstra = graph_dijkstra(g, &d);
	graph_print_dijkstra_data(stdout, dijkstra);

	dijkstra = graph_dijkstra(g, &e);
	graph_print_dijkstra_data(stdout, dijkstra);


	dijkstra = graph_dijkstra(g, cast_char('F'));
	assert(dijkstra.status == ELEMENT_NOT_FOUND_ERROR);

	printf("\n****************************************************\n");
	printf("Here bellow should appear a NULL parameter error.\n");
	dijkstra = graph_dijkstra(NULL, NULL);
	assert(dijkstra.status == NULL_PARAMETER_ERROR);
	printf("****************************************************\n");

	graph_free(g);
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


	TIMESTAMP_STOP
	graph_free(g);
	END_MSG(Graph)
	return 0;
}