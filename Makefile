.PHONY: default clean purge libs test install uninstall

SRC = src
BIN = bin
LIB = lib
INCLUDE = include

CFILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OFILES = $(patsubst %.c,%.o,$(CFILES))
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

INSTALL_PATH ?= /usr/local
HEADER_MSG = "// Source code: https://git.saulv.es/Generic-Data-Structures"

libs: $(OFILES) | $(LIB)/  $(INCLUDE)/ $(INCLUDE)/util/
	$(info Building libs ...)
	@ $(CC) $(CCFLAGS) -shared -o ./$(LIB)/libGDS.so $(OFILES)
	@ $(AR) $(ARFLAGS) ./$(LIB)/libGDS-static.a $(OFILES)
	@ $(foreach H,$(wildcard $(SRC)/*.h), echo $(HEADER_MSG) | cat - $(H) | cat - > $(INCLUDE)/$(notdir $(H));)
	@ $(foreach H,$(wildcard $(SRC)/util/*.h), echo $(HEADER_MSG) | cat - $(H) | cat - > $(INCLUDE)/util/$(notdir $(H));)

install: libs
	@ sudo su -c '\
	  echo "Installing GDS in $(INSTALL_PATH) ..." ;\
	  install -d $(INSTALL_PATH)/lib ;\
	  install -m 644 $(LIB)/* $(INSTALL_PATH)/lib ;\
	  install -d $(INSTALL_PATH)/include/GDS ;\
	  install -d $(INSTALL_PATH)/include/GDS/util ;\
	  install -m 644 $(INCLUDE)/*.h $(INSTALL_PATH)/include/GDS ;\
	  install -m 644 $(INCLUDE)/util/*.h $(INSTALL_PATH)/include/GDS/util ;\
	  ldconfig $(INSTALL_PATH)/lib '

uninstall:
	@ sudo su -c '\
	  echo "Uninstalling GDS ..." ;\
	  rm -f $(INSTALL_PATH)/lib/libGDS* ;\
	  rm -rf $(INSTALL_PATH)/include/GDS ;\
	  ldconfig $(INSTALL_PATH)/lib '

test: $(TESTFILES) $(LIB)/libGDS-static.a | $(BIN)/
	$(info Running tests ...)
	@ $(foreach T,$(filter %.c,$(TESTFILES)), \
	  $(CC) $(CCFLAGS) -o $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) $(T) -L./$(LIB)/ -lGDS-static; \
	  $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) || exit 1;)

$(LIB)/libGDS-static.a: libs

.c.o:
	$(CC) $(CCFLAGS) -o $@ -c $<

%/:
	@ mkdir $@

clean:
	@ find . -type f -name '*.o' -delete
	@ rm -rf $(BIN)/
	@ rm -rf $(LIB)/
	@ rm -rf $(INCLUDE)/
