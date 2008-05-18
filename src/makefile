# CFG   = [rel|dbg|small] - dbg if not given

HOST_LINUX := Linux
HOST_MAC := Darwin

# HOST can be: Linux, Darwin, CYGWIN_NT-6.0, CYGWIN_NT-5.1
HOST := $(shell uname -s)

$(info )
$(info Building on $(shell uname -a))
$(info gcc version: $(shell $(CC) -dumpversion))
$(info gcc machine: $(shell $(CC) -dumpmachine))
$(info )

ifeq ($(CFG),)
$(warning Configuration not specified - using default debug configuration)
CFG=dbg
endif

INCS = -I .

CFLAGS += -g

ifeq ($(CFG),rel)
CFLAGS += -O2
endif

ifeq ($(CFG),dbg)
CFLAGS += -O0
endif

ifeq ($(CFG),small)
CFLAGS += -Os
endif

LDFLAGS += -lstdc++

ifeq ($(HOST),$(HOST_MAC))
LDFLAGS += -dead_strip
endif

OUTDIR=obj-$(CFG)
OUTDIR_EXISTS = $(OUTDIR)/.ensure_exists
O=$(OUTDIR)

all: $(O)/parsed_str_dummy_stripped $(O)/parsed_str_stl_stripped \
	$(O)/parsed_str_unopt_stripped $(O)/parsed_str_opt_alloc_stripped

rebuild: force
	make all CFG=small
	make all CFG=rel
	make all CFG=dbg

$(OUTDIR_EXISTS):
	mkdir -p $(OUTDIR)
	touch $(OUTDIR_EXISTS)

$(OUTDIR)/parsed_str_dummy: $(OUTDIR_EXISTS) force
	gcc $(CFLAGS) $(LDFLAGS) ParsedStrTest.cpp ParsedStrDummy.cpp -DVER_DUMMY -o $@

$(OUTDIR)/parsed_str_dummy_stripped: $(OUTDIR)/parsed_str_dummy
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_stl: $(OUTDIR_EXISTS) force
	gcc $(CFLAGS) $(LDFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrStl.cpp -DVER_STL -o $@

$(OUTDIR)/parsed_str_stl_stripped: $(OUTDIR)/parsed_str_stl
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_unopt: $(OUTDIR_EXISTS) force
	gcc $(CFLAGS) $(LDFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrUnopt.cpp -DVER_UNOPT -o $@

$(OUTDIR)/parsed_str_unopt_stripped: $(OUTDIR)/parsed_str_unopt
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_opt_alloc: $(OUTDIR_EXISTS) force
	gcc $(CFLAGS) $(LDFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrOptAlloc.cpp -DVER_OPT_ALLOC -o $@

$(OUTDIR)/parsed_str_opt_alloc_stripped: $(OUTDIR)/parsed_str_opt_alloc
	strip -S -o $@ $<

inform:
ifneq ($(CFG),rel)
ifneq ($(CFG),dbg)
ifneq ($(CFG),small)
	@echo "Invalid configuration: '"$(CFG)"'"
	@echo "Valid configurations: rel, dbg, small (e.g. make CFG=dbg)"
	@exit 1
endif
endif
endif

force:

clean:
	rm -rf $(OUTDIR)

cleanall:
	rm -rf obj-*

.phony: inform clean cleanall
