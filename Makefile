.PHONY: default clean purge libs test install uninstall doxygen

SRC = src
BIN = bin

CFILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OFILES = $(patsubst %.c,%.o,$(CFILES))
TESTFILES = $(wildcard test/*)

CC = gcc
CCFLAGS = -Wall -Wextra -pedantic -std=c99 -Wstrict-prototypes -g -fPIC -O3

# User's custom flags
ifdef FLAGS
CCFLAGS += $(FLAGS)
endif

AR = ar
ARFLAGS = rcs

INSTALL_PATH ?= /usr/local

default: libs

libs: $(OFILES) | $(BIN)/
	@ $(CC) $(CCFLAGS) -shared -o ./$(BIN)/libGDS.so $(OFILES)
	@ $(AR) $(ARFLAGS) ./$(BIN)/libGDS-static.a $(OFILES)

install: libs
	@ sudo su -c '\
	  echo "Installing GDS in $(INSTALL_PATH) ..." ;\
	  install -d $(INSTALL_PATH)/lib ;\
	  install -m 644 $(BIN)/libGDS* $(INSTALL_PATH)/lib ;\
	  install -d $(INSTALL_PATH)/include/GDS ;\
	  install -d $(INSTALL_PATH)/include/GDS/util ;\
	  install -m 644 $(SRC)/*.h $(INSTALL_PATH)/include/GDS ;\
	  install -m 644 $(SRC)/util/*.h $(INSTALL_PATH)/include/GDS/util ;\
	  ldconfig $(INSTALL_PATH)/lib '

uninstall:
	@ sudo su -c '\
	  echo "Uninstalling GDS ..." ;\
	  rm -f $(INSTALL_PATH)/lib/libGDS* ;\
	  rm -rf $(INSTALL_PATH)/include/GDS ;\
	  ldconfig $(INSTALL_PATH)/lib '

NO-RUN?= false # If true, only builds the test, without running them
test: $(TESTFILES) libs | $(BIN)/
	@ $(foreach T,$(filter %.c,$(TESTFILES)), \
	  $(CC) $(CCFLAGS) -o $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) $(T) -L./$(BIN)/ -lGDS-static; \
	  $(NO-RUN) || $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) || exit 1;)

doxygen: ./doxygen/
	@ echo "/**" > doxygen/doc.doxy
	@ find ./src -name '*.h' -exec bash -c ' \
		name={} ; base=$$(basename $$name) ; \
		echo -n " * @file $$name " >> doxygen/doc.doxy ; \
		cat $$name | grep $$base | sed "s/.*$$base -//"' >> doxygen/doc.doxy \;
	@ echo "*/" >> doxygen/doc.doxy
	doxygen .doxyfile
	@ rm -f doxygen/doc.doxy

.c.o:
	$(CC) $(CCFLAGS) -o $@ -c $<

%/:
	@ mkdir $@

clean:
	@ rm -f $(OFILES)
	@ rm -rf doxygen
