.PHONY: default clean libs test examples install uninstall doxygen

SRC = src
BIN = bin

CFILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OFILES = $(patsubst %.c,%.o,$(CFILES))
TESTFILES = $(wildcard test/*)
EXAMPLES = $(wildcard example/*.c)

PROFILE := release

CC := cc

DISABLED_WARNINGS_LIST = nonnull-compare unknown-warning-option pointer-bool-conversion

DISABLED_WARNINGS = $(foreach W,$(DISABLED_WARNINGS_LIST), -Wno-$(W))

CCFLAGS += -Wall -Wextra -pedantic -Wstrict-prototypes $(DISABLED_WARNINGS) \
			-std=c11 -I./include -g -fPIC $(FLAGS)

ifeq ($(PROFILE),release)
	CCFLAGS += -O3
else ifeq ($(PROFILE),coverage)
	CCFLAGS += --coverage
endif

AR = ar
ARFLAGS = rcs

PREFIX ?= /usr/local

default: libs

libs: $(OFILES) | $(BIN)/
	@ echo " => $(BIN)/libGDS.so"
	@ $(CC) $(CCFLAGS) -shared -o ./$(BIN)/libGDS.so $(OFILES)
	@ echo " => $(BIN)/libGDS-static.a"
	@ $(AR) $(ARFLAGS) ./$(BIN)/libGDS-static.a $(OFILES)

install: $(BIN)/libGDS.so $(BIN)/libGDS-static.a
	  @ echo " INSTALL $(PREFIX)/lib/libGDS.so"
	  @ echo " INSTALL $(PREFIX)/lib/libGDS-static.a"
	  @ mkdir -p -m 755 $(PREFIX)/lib
	  @ cp $(BIN)/libGDS* $(PREFIX)/lib
	  @ chmod 644 $(PREFIX)/lib/libGDS*
	  @ echo " INSTALL $(PREFIX)/include/GDS/"
	  @ mkdir -p -m 755 $(PREFIX)/include
	  @ cp -r include $(PREFIX)/include/GDS
	  @ chmod -R 644 $(PREFIX)/include/GDS
	  @ find $(PREFIX)/include/GDS -type d -exec chmod 755 {} \;

$(BIN)/libGDS.so $(BIN)/libGDS-static.a:
	@ $(MAKE) --no-print-directory libs

uninstall:
	  @ echo " RM $(PREFIX)/lib/libGDS.so"
	  @ echo " RM $(PREFIX)/lib/libGDS-static.a"
	  @ rm -f $(PREFIX)/lib/libGDS*
	  @ echo " RM $(PREFIX)/include/GDS/"
	  @ rm -rf $(PREFIX)/include/GDS

NO-RUN?= false # If true, only builds the test, without running them
test: $(TESTFILES) libs | $(BIN)/
	@ $(foreach T,$(filter %.c,$(TESTFILES)), \
	  $(CC) $(CCFLAGS) -o $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) $(T) -L./$(BIN)/ -lGDS-static; \
	  $(NO-RUN) || $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) || exit 1;)

examples: $(EXAMPLES) $(OFILES) | $(BIN)/
	@ $(foreach F,$(EXAMPLES), \
		echo " CC $(F)" ; \
		$(CC) $(CCFLAGS) -o $(patsubst example/%.c,bin/%,$(F)) $(F) $(OFILES) ;)

doxygen: ./doxygen/
	@ echo -e "\
	/** @mainpage \n \
		@verbinclude README.adoc */" > ./doxygen/doc.doxy
	@ echo "/**" >> doxygen/doc.doxy
	@ find ./include -name '*.h' -exec bash -c ' \
		name={} ; base=$$(basename $$name) ; \
		echo -n " * @file $$name " >> doxygen/doc.doxy ; \
		cat $$name | grep $$base | sed "s/.*$$base -//"' >> doxygen/doc.doxy \;
	@ echo "*/" >> doxygen/doc.doxy
	doxygen .doxyfile
	@ rm -f doxygen/doc.doxy

.c.o:
	@ echo " CC $@"
	@ $(CC) $(CCFLAGS) -o $@ -c $<

%/:
	@ mkdir $@

clangd:
	@ echo -e \
		"CompileFlags: \n" \
		"Add: [ -I$(shell pwd)/include/ , -xc " \
		"$(foreach W,$(DISABLED_WARNINGS), , $(W))" "]" \
		> .clangd

clean:
	@ rm -f $(OFILES)
	@ find . \( -name '*.*.gcov' -o -name '*.gcda' -o -name '*.gcno' \) -delete
	@ rm -f README.html
	@ rm -rf doxygen bin
