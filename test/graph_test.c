#define COMPARATOR_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#include "../src/Graph/graph.h"

int main(){
        Graph g = graph_init(15, Comparator.integer);
        printf("Correctly Inited Graph\n");
        return 0;
}