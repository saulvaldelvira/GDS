.PHONY: clean list_test
SRC = src/
UTIL_DIR = $(SRC)/Util
BIN_DIR = bin/
LIST_OBJS = test/list_test.o $(SRC)/ArrayList/array_list.o $(SRC)/LinkedList/linked_list.o $(UTIL_DIR)/comparator.o $(UTIL_DIR)/allocate.o

CC = gcc
CCFLAGS = -lm

list_test: $(BIN_DIR) $(LIST_OBJS)
	$(CC) -o $(BIN_DIR)/list_test.out $(LIST_OBJS) $(CCFLAGS)

.c.o:
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.out' -delete
	find . -type f -name '*.exe' -delete
