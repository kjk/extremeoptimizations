# CFG   = [rel|dbg|small] - dbg if not given

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

OUTDIR=obj-$(CFG)
OUTDIR_EXISTS = $(OUTDIR)/.ensure_exists

all: $(OUTDIR)/parsed_str_dummy $(OUTDIR)/parsed_str_stl_stripped

rebuild: force
	make all CFG=small
	make all CFG=rel
	make all CFG=dbg

$(OUTDIR_EXISTS):
	mkdir -p $(OUTDIR)
	touch $(OUTDIR_EXISTS)

$(OUTDIR)/parsed_str_dummy: $(OUTDIR_EXISTS) force
	gcc $(CFLAGS) $(LDFLAGS) ParsedStrTest.cpp ParsedStrStl.cpp -DVER_DUMMY -o $@

$(OUTDIR)/parsed_str_dummy_stripped: $(OUTDIR)/parsed_str_dummy
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_stl: $(OUTDIR_EXISTS) force
	gcc $(CFLAGS) $(LDFLAGS) ParsedStrTest.cpp ParsedStrStl.cpp -DVER_STL -o $@

$(OUTDIR)/parsed_str_stl_stripped: $(OUTDIR)/parsed_str_stl
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
