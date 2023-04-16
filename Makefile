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

INSTALL_PATH ?= /usr/local

libs: $(OFILES) | $(LIB)/  $(INCLUDE)/ $(INCLUDE)/Util/
	$(info Building libs ...)
	@ gcc $(CCFLAGS) -shared -o ./$(LIB)/libGDS.so $(OFILES)
	@ $(AR) $(ARFLAGS) ./$(LIB)/libGDS-static.a $(OFILES)
	@ $(foreach H,$(wildcard $(SRC)/*.h), echo $(HEADER_MSG) | cat - $(H) | cat - > $(INCLUDE)/$(notdir $(H));)
	@ $(foreach H,$(wildcard $(SRC)/Util/*.h), echo $(HEADER_MSG) | cat - $(H) | cat - > $(INCLUDE)/Util/$(notdir $(H));)
	@ chown $(logname):$(logname) -R $(LIB)/ $(INCLUDE)/

install: libs
	$(info Installing GDS in $(INSTALL_PATH)/lib ...)
	@ install -d $(INSTALL_PATH)/lib
	@ install -m 644 $(LIB)/libGDS.so $(INSTALL_PATH)/lib
	@ install -m 644 $(LIB)/libGDS-static.a $(INSTALL_PATH)/lib
	@ install -d $(INSTALL_PATH)/include/GDS
	@ install -d $(INSTALL_PATH)/include/GDS/Util
	@ install -m 644 $(INCLUDE)/*.h $(INSTALL_PATH)/include/GDS
	@ install -m 644 $(INCLUDE)/Util/*.h $(INSTALL_PATH)/include/GDS/Util
	@ ldconfig $(INSTALL_PATH)/lib

uninstall:
	$(info Uninstalling GDS ...)
	@ rm -f $(INSTALL_PATH)/lib/libGDS.so
	@ rm -f $(INSTALL_PATH)/lib/libGDS-static.a
	@ rm -rf $(INSTALL_PATH)/include/GDS
	@ ldconfig $(INSTALL_PATH)/lib

# Build and run all test programs-f
test: $(TESTFILES) $(LIB)/libGDS-static.a | $(BIN)/
	$(info Running tests ...)
	@ $(foreach T,$(filter %.c,$(TESTFILES)), \
	  $(CC) $(CCFLAGS) -o $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) $(T) -L./$(LIB)/ -lGDS-static; \
	  $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) || exit 1;)

$(LIB)/libGDS-static.a: libs

.c.o:
	$(CC) $(CCFLAGS) -c -o $@ $<

%/:
	@ mkdir $@

clean:
	$(info Cleaning ...)
	@ find . -type f -name '*.o' -delete
	@ find . -type f -name 'cachegrind.out.*' -delete
	@ find . -type f -name '*.orig' -delete
	@ find . -type f -name '*_BACKUP_*' -delete
	@ find . -type f -name '*_BASE_*' -delete
	@ find . -type f -name '*_LOCAL_*' -delete
	@ find . -type f -name '*_REMOTE_*' -delete
	@ find . -type f -name 'vgcore*' -delete

purge: clean
	$(info Purging ...)
	@ rm -rf $(BIN)/
	@ rm -rf $(LIB)/
	@ rm -rf $(INCLUDE)/
