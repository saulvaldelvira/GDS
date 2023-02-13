.PHONY: default clean purge all_test list_test stack_test queue_test bst_test binary_heap
SRC = src/
UTIL_DIR = $(SRC)/Util
BIN = bin/
UTIL_OBJS = $(UTIL_DIR)/comparator.o
LIST_OBJS = test/list_test.o $(SRC)/Vector/dynamic_array.o $(SRC)/LinkedList/linked_list.o $(UTIL_OBJS)
STACK_OBJS = test/stack_test.o $(SRC)/Stack/stack.o $(UTIL_OBJS)
QUEUE_OBJS = test/queue_test.o $(SRC)/Queue/queue.o $(UTIL_OBJS)
BST_OBJS = test/bst_test.o $(SRC)/BSTree/BSTree.o $(UTIL_OBJS)
AVL_OBJS = test/avl_test.o $(SRC)/AVLTree/AVLTree.o $(UTIL_OBJS)
GRAPH_OBJS = test/graph_test.o $(SRC)/Graph/graph.o $(UTIL_OBJS)
BINHEAP_OBJS = test/heap_test.o $(SRC)/Vector/dynamic_array.o $(SRC)/BinaryHeap/binary_heap_min.o $(UTIL_OBJS)
BTREE_OBJS = test/btree_test.o $(SRC)/BTree/BTree.o $(UTIL_OBJS)

CC = gcc
CCFLAGS = -lm -Wall -Wextra -Werror -g3 -pedantic

# To set optimization level append "O=X" in make command. X is the optimiztion level. 
ifdef O
CCFLAGS += -O$(O)
endif

# To make the test NOT print full status in console
ifdef QUIET
CCFLAGS += -DQUIET
endif

ifdef DEBUG
CCFLAGS += -DDEBUG
endif

default: all_test

list_test: $(BIN) $(BIN)/list_test.out
	@ $(BIN)/list_test.out 

stack_test: $(BIN) $(BIN)/stack_test.out
	@ $(BIN)/stack_test.out 

queue_test: $(BIN) $(BIN)/queue_test.out
	@ $(BIN)/queue_test.out 

bst_test: $(BIN) $(BIN)/bst_test.out
	@ $(BIN)/bst_test.out

avl_test: $(BIN) $(BIN)/avl_test.out
	@ $(BIN)/avl_test.out

graph_test: $(BIN) $(BIN)/graph_test.out
	@ $(BIN)/graph_test.out

binary_heap: $(BIN) $(BIN)/heap_test.out
	@ $(BIN)/heap_test.out

btree_test: $(BIN) $(BIN)/btree_test.out
	@ $(BIN)/btree_test.out

$(BIN)/list_test.out: $(LIST_OBJS)
	@ $(CC) -o $(BIN)/list_test.out $(LIST_OBJS) $(CCFLAGS)

$(BIN)/stack_test.out:  $(STACK_OBJS)
	@ $(CC) -o $@ $(STACK_OBJS) $(CCFLAGS)

$(BIN)/queue_test.out:  $(QUEUE_OBJS)
	@ $(CC) -o $@ $(QUEUE_OBJS) $(CCFLAGS)

$(BIN)/bst_test.out: $(BST_OBJS)
	@ $(CC) -o $@ $(BST_OBJS) $(CCFLAGS)

$(BIN)/avl_test.out: $(AVL_OBJS)
	@ $(CC) -o $@ $(AVL_OBJS) $(CCFLAGS)

$(BIN)/graph_test.out: $(GRAPH_OBJS)
	@ $(CC) -o $@ $(GRAPH_OBJS) $(CCFLAGS)

$(BIN)/heap_test.out: $(BINHEAP_OBJS)
	@ $(CC) -o $@ $(BINHEAP_OBJS) $(CCFLAGS)

$(BIN)/btree_test.out: $(BTREE_OBJS)
	@ $(CC) -o $@ $(BTREE_OBJS) $(CCFLAGS)

.c.o:
	@ $(CC) $(CCFLAGS) -c -o $@ $<

$(BIN):
	@ mkdir $(BIN)

all_test: list_test stack_test queue_test bst_test graph_test btree_test

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name 'cachegrind.out.*' -delete
	find . -type f -name '*.orig' -delete
	find . -type f -name '*_BACKUP_*' -delete
	find . -type f -name '*_BASE_*' -delete
	find . -type f -name '*_LOCAL_*' -delete
	find . -type f -name '*_REMOTE_*' -delete

purge: clean
	find . -type f -name '*.out' -delete
	find . -type f -name '*.exe' -delete
	rm -rf bin/
