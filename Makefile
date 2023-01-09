.PHONY: clean all_test list_test stack_test queue_test bst_test 
SRC = src/
UTIL_DIR = $(SRC)/Util
BIN = bin/
UTIL_OBJS = $(UTIL_DIR)/comparator.o
LIST_OBJS = test/list_test.o $(SRC)/ArrayList/array_list.o $(SRC)/LinkedList/linked_list.o $(UTIL_OBJS)
STACK_OBJS = test/stack_test.o $(SRC)/Stack/stack.o $(UTIL_OBJS)
QUEUE_OBJS = test/queue_test.o $(SRC)/Queue/queue.o $(UTIL_OBJS)
BST_OBJS = test/bst_test.o $(SRC)/BSTree/BSTree.o $(UTIL_OBJS)
GRAPH_OBJS = test/graph_test.o $(SRC)/Graph/graph.o $(UTIL_OBJS)

CC = gcc
CCFLAGS = -lm -Wall -Wextra -Werror -g

list_test: $(BIN) $(BIN)/list_test.out
	@ $(BIN)/list_test.out 

stack_test: $(BIN) $(BIN)/stack_test.out
	@ $(BIN)/stack_test.out 

queue_test: $(BIN) $(BIN)/queue_test.out
	@ $(BIN)/queue_test.out 

bst_test: $(BIN) $(BIN)/bst_test.out
	@ $(BIN)/bst_test.out

graph_test: $(BIN) $(BIN)/graph_test.out
	@ $(BIN)/graph_test.out

$(BIN)/list_test.out: $(LIST_OBJS)
	@ $(CC) -o $(BIN)/list_test.out $(LIST_OBJS) $(CCFLAGS)

$(BIN)/stack_test.out:  $(STACK_OBJS)
	@ $(CC) -o $@ $(STACK_OBJS) $(CCFLAGS)

$(BIN)/queue_test.out:  $(QUEUE_OBJS)
	@ $(CC) -o $@ $(QUEUE_OBJS) $(CCFLAGS)

$(BIN)/bst_test.out: $(BST_OBJS)
	@ $(CC) -o $@ $(BST_OBJS) $(CCFLAGS)

$(BIN)/graph_test.out: $(GRAPH_OBJS)
	@ $(CC) -o $@ $(GRAPH_OBJS) $(CCFLAGS)

.c.o:
	@ $(CC) $(CCFLAGS) -c -o $@ $<

$(BIN):
	@ mkdir $(BIN)

all_test: list_test stack_test queue_test bst_test

clean:
	find . -type f -name '*.o' -delete

purge: clean
	find . -type f -name '*.out' -delete
	find . -type f -name '*.exe' -delete
