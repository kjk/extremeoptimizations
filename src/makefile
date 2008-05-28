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
CFLAGS += -O2 -DNDEBUG
endif

ifeq ($(CFG),dbg)
CFLAGS += -O0
endif

ifeq ($(CFG),small)
CFLAGS += -Os -DNDEBUG
endif

LDFLAGS += -lstdc++

ifeq ($(HOST),$(HOST_MAC))
LDFLAGS += -dead_strip
endif

OUTDIR=obj-$(CFG)
OUTDIR_EXISTS = $(OUTDIR)/.ensure_exists
O=$(OUTDIR)

ifeq ($(STRIP),yes)
all: $(O)/temp_alloc_test_stripped \
	$(O)/parsed_str_dummy_stripped $(O)/parsed_str_stl_stripped \
	$(O)/parsed_str_unopt_stripped $(O)/parsed_str_opt_alloc_stripped \
	$(O)/parsed_str_opt_one_array_stripped $(O)/parsed_str_opt_offsets_stripped \
	$(O)/parsed_str_opt_no_offsets_stripped $(O)/parsed_str_opt_common_stripped \
	$(O)/parsed_str_opt_no_offsets_no_dup_stripped
else
all: $(O)/temp_alloc_test \
	$(O)/parsed_str_dummy $(O)/parsed_str_stl \
	$(O)/parsed_str_unopt $(O)/parsed_str_opt_alloc \
	$(O)/parsed_str_opt_one_array $(O)/parsed_str_opt_offsets \
	$(O)/parsed_str_opt_no_offsets $(O)/parsed_str_opt_common \
	$(O)/parsed_str_opt_no_offsets_no_dup
endif

rebuild: force
	make all CFG=small
	make all CFG=rel
	make all CFG=dbg

$(OUTDIR_EXISTS):
	mkdir -p $(OUTDIR)
	touch $(OUTDIR_EXISTS)

$(OUTDIR)/temp_alloc_test: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) temp_alloc_test.c temp_alloc.c $(LDFLAGS) -o $@

$(OUTDIR)/temp_alloc_test_stripped: $(OUTDIR)/temp_alloc_test
	strip -S -o $@ $<


$(OUTDIR)/parsed_str_dummy: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp ParsedStrDummy.cpp -DVER_DUMMY $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_dummy_stripped: $(OUTDIR)/parsed_str_dummy
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_stl: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrStl.cpp -DVER_STL $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_stl_stripped: $(OUTDIR)/parsed_str_stl
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_unopt: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrUnopt.cpp -DVER_UNOPT $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_unopt_stripped: $(OUTDIR)/parsed_str_unopt
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_opt_alloc: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrOptAlloc.cpp -DVER_OPT_ALLOC $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_opt_alloc_stripped: $(OUTDIR)/parsed_str_opt_alloc
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_opt_one_array: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrOptOneArray.cpp -DVER_OPT_ONE_ARRAY $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_opt_one_array_stripped: $(OUTDIR)/parsed_str_opt_one_array
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_opt_offsets: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrOptOffsets.cpp $(LDFLAGS) -DVER_OPT_OFFSETS -o $@

$(OUTDIR)/parsed_str_opt_offsets_stripped: $(OUTDIR)/parsed_str_opt_offsets
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_opt_no_offsets: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrOptNoOffsets.cpp -DVER_OPT_NO_OFFSETS $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_opt_no_offsets_stripped: $(OUTDIR)/parsed_str_opt_no_offsets
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_opt_no_offsets_no_dup: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrOptNoOffsetsNoDup.cpp -DVER_OPT_NO_OFFSETS_NO_DUP $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_opt_no_offsets_no_dup_stripped: $(OUTDIR)/parsed_str_opt_no_offsets_no_dup
	strip -S -o $@ $<

$(OUTDIR)/parsed_str_opt_common: $(OUTDIR_EXISTS) force
	cc $(CFLAGS) ParsedStrTest.cpp  ParsedStrUtil.cpp ParsedStrOptCommon.cpp -DVER_OPT_COMMON $(LDFLAGS) -o $@

$(OUTDIR)/parsed_str_opt_common_stripped: $(OUTDIR)/parsed_str_opt_common
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
