.PHONY: clean list_test
SRC = DataStructures/
# LISTDIR = $(SRC)/List
# LIST_OBJS = $(LISTDIR)/list.o $(LISTDIR)/linked_list.o
BIN_DIR = bin/

list_test: $(BIN_DIR)
	gcc $(SRC)/list_test.c $(SRC)/comparator.c $(SRC)/allocate.c $(SRC)/List/*.c -o bin/list_test.out

$(BIN_DIR):
	mkdir $(BIN_DIR)

clean:
	rm -f src/*/*.o
	rm -f src/*/*.out
	rm -f src/*/*.exe
	rm -f src/*.o
	rm -f src/*.out 
	rm -f src/*.exe
	rm -f bin/*.o
	rm -f bin/*.out
	rm -f bin/*.exe
