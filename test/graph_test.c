#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE
#include "../src/Graph/graph.h"

int main(){
        intptr_t n = 1200;
        printf("[Graph Test]\n");
        TIMESTAMP_START
        Graph *g = graph_empty(sizeof(int) ,compare_int);
        for(int i = 0; i < n; i++){
                assert(graph_add_node(g, &i));
                assert(graph_n_elements(g) == (size_t) i+1);
        }
        for(int i = 0; i < n; i++){
                assert(graph_exists(g, &i));
                assert(graph_remove_node(g, &i));
        }
        TIMESTAMP_STOP
        graph_free(g);
        END_MSG(Graph)
        return 0;
}