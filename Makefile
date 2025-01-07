.PHONY: default clean libs test examples install uninstall doxygen

SRC = src
BIN = bin

CFILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OFILES = $(patsubst %.c,%.o,$(CFILES))
TESTFILES = $(wildcard test/*)
BENCH_FILES = $(wildcard benches/*)
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

INSTALL_PATH ?= /usr/local

default: libs

libs: $(OFILES) | $(BIN)/
	@ echo " => $(BIN)/libGDS.so"
	@ $(CC) $(CCFLAGS) -shared -o ./$(BIN)/libGDS.so $(OFILES)
	@ echo " => $(BIN)/libGDS-static.a"
	@ $(AR) $(ARFLAGS) ./$(BIN)/libGDS-static.a $(OFILES)

install: $(BIN)/libGDS.so $(BIN)/libGDS-static.a
	  @ echo " INSTALL $(INSTALL_PATH)/lib/libGDS.so"
	  @ echo " INSTALL $(INSTALL_PATH)/lib/libGDS-static.a"
	  @ mkdir -p -m 755 $(INSTALL_PATH)/lib
	  @ cp $(BIN)/libGDS* $(INSTALL_PATH)/lib
	  @ chmod 644 $(INSTALL_PATH)/lib/libGDS*
	  @ echo " INSTALL $(INSTALL_PATH)/include/GDS/"
	  @ mkdir -p -m 755 $(INSTALL_PATH)/include
	  @ cp -r include $(INSTALL_PATH)/include/GDS
	  @ chmod -R 644 $(INSTALL_PATH)/include/GDS
	  @ find $(INSTALL_PATH)/include/GDS -type d -exec chmod 755 {} \;

$(BIN)/libGDS.so $(BIN)/libGDS-static.a:
	@ $(MAKE) --no-print-directory libs

uninstall:
	  @ echo " RM $(INSTALL_PATH)/lib/libGDS.so"
	  @ echo " RM $(INSTALL_PATH)/lib/libGDS-static.a"
	  @ rm -f $(INSTALL_PATH)/lib/libGDS*
	  @ echo " RM $(INSTALL_PATH)/include/GDS/"
	  @ rm -rf $(INSTALL_PATH)/include/GDS

NO-RUN?= false # If true, only builds the test, without running them
test: $(TESTFILES) libs | $(BIN)/
	@ $(foreach T,$(filter %.c,$(TESTFILES)), \
	  $(CC) $(CCFLAGS) -o $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) $(T) -L./$(BIN)/ -lGDS-static; \
	  $(NO-RUN) || $(BIN)/$(patsubst %.c,%.out, $(notdir $(T))) || exit 1;)

bench: $(BENCH_FILES) libs | $(BIN)/
	 @ $(foreach T,$(filter %.c,$(BENCH_FILES)), \
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
