.PHONY: clean list_test stack_test test_all
SRC = src/
UTIL_DIR = $(SRC)/Util
BIN = bin/
UTIL_OBJS = $(UTIL_DIR)/comparator.o $(UTIL_DIR)/allocate.o
LIST_OBJS = test/list_test.o $(SRC)/ArrayList/array_list.o $(SRC)/LinkedList/linked_list.o $(UTIL_OBJS)

STACK_OBJS = test/stack_test.o $(SRC)/Stack/stack.o $(UTIL_OBJS)

CC = gcc
CCFLAGS = -lm

list_test: $(BIN) $(BIN)/list_test.out

stack_test: $(BIN) $(BIN)/stack_test.out

$(BIN)/list_test.out: $(LIST_OBJS)
	$(CC) -o $(BIN)/list_test.out $(LIST_OBJS) $(CCFLAGS)

$(BIN)/stack_test.out:  $(STACK_OBJS)
	$(CC) -o $@ $(STACK_OBJS) $(CCFLAGS)

.c.o:
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BIN):
	mkdir $(BIN)

test_all: list_test stack_test
	@ $(BIN)/list_test.out
	@ $(BIN)/stack_test.out

clean:
	find . -type f -name '*.o' -delete

purge: clean
	find . -type f -name '*.out' -delete
	find . -type f -name '*.exe' -delete
