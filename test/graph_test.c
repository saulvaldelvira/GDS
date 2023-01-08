#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE
#include "../src/Graph/graph.h"

int main(){
        intptr_t n = 12000;
        printf("[Graph Test]\n");
        TIMESTAMP_START
        Graph g = graph_init(Comparator.literal_integer);
        for(intptr_t i = 0; i < n; i++){
                assert(graph_add_node(&g, (void*) i));
                assert(g.n_elements == (size_t) i+1);
        }
        for(intptr_t i = 0; i < n; i++){
                assert(graph_exists(g, (void*) i));
        }
        TIMESTAMP_STOP
        graph_free(g);
        END_MSG(Graph)
        return 0;
}