.PHONY: default clean purge libs test

SRC = src
BIN = bin
LIB = lib
INCLUDE = include

CFILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OFILES = $(patsubst %.c,%.o,$(CFILES))
LIBFILES = $(addprefix $(LIB)/lib,$(notdir $(CFILES:.c=.so)))
TESTFILES = $(wildcard test/*)

CC = gcc
CCFLAGS = -Wall -Wextra -Werror -pedantic -g -fPIC -O3

# User's custom flags
ifdef FLAGS
CCFLAGS += $(FLAGS)
endif

AR = ar
ARFLAGS = rcs

default: libs

libs: $(OFILES) | $(LIB)/  $(INCLUDE)/ $(INCLUDE)/Util/
	$(info Building libs...)
	@ gcc $(CCFLAGS) -shared -o ./$(LIB)/libGDS.so $(OFILES)
	@ $(AR) $(ARFLAGS) ./$(LIB)/libGDS-static.a $(OFILES)
	@ $(foreach O,$(OFILES),$(AR) $(ARFLAGS) ./$(LIB)/lib$(patsubst %.o,%.a, $(notdir $(O))) $(O) ;)
	@ cp $(SRC)/*.h $(INCLUDE)/ && cp $(SRC)/Util/*.h $(INCLUDE)/Util/

# Build and run all test programs
test: $(TESTFILES) $(LIB)/libGDS-static.a | $(BIN)/
	$(info Running tests...)
	@ $(foreach T,$(filter %.c,$(TESTFILES)), \
	  $(CC) $(CCFLAGS) -o $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) $(T) -L./$(LIB)/ -lGDS-static; \
	  $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) ;)

$(LIB)/libGDS-static.a: libs

.c.o:
	$(CC) $(CCFLAGS) -c -o $@ $<

%/:
	@ mkdir $@

clean:
	$(info Cleaning...)
	@ find . -type f -name '*.o' -delete
	@ find . -type f -name 'cachegrind.out.*' -delete
	@ find . -type f -name '*.orig' -delete
	@ find . -type f -name '*_BACKUP_*' -delete
	@ find . -type f -name '*_BASE_*' -delete
	@ find . -type f -name '*_LOCAL_*' -delete
	@ find . -type f -name '*_REMOTE_*' -delete
	@ find . -type f -name 'vgcore*' -delete

purge: clean
	$(info Purging...)
	@ rm -rf $(BIN)/
	@ rm -rf $(LIB)/
