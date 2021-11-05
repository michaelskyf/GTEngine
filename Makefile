# SPDX-License-Identifier: GPL-2.0
export PBUILD_PROJECTNAME = gltest

$(if $(filter __%, $(MAKECMDGOALS)), \
	$(error targets prefixed with '__' are only for internal use))

# That's our default target when none is given on the command line
PHONY := __all
__all:

ifneq ($(sub_make_done), 1)

# Do not use make's built-in rules and variables
# (this increases performance and avoids hard-to-debug behaviour)
MAKEFLAGS += -rR

# Avoid interference with shell env settings
unexport GREP_OPTIONS

# Beautify output
# ---------------------------------------------------------------------------
#
# Normally, we echo the whole command before executing it. By making
# that echo $($(quiet)$(cmd)), we now have the possibility to set
# $(quiet) to choose other forms of output instead, e.g.
#
#         quiet_cmd_cc_o_c = Compiling $(RELDIR)/$@
#         cmd_cc_o_c       = $(CC) $(c_flags) -c -o $@ $<
#
# If $(quiet) is empty, the whole command will be printed.
# If it is set to "quiet_", only the short version will be printed.
# If it is set to "silent_", nothing will be printed at all, since
# the variable $(silent_cmd_cc_o_c) doesn't exist.
#
# A simple variant is to prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)ln $@ :<
#
# If KBUILD_VERBOSE equals 0 then the above command will be hidden.
# If KBUILD_VERBOSE equals 1 then the above command is displayed.
# If KBUILD_VERBOSE equals 2 then give the reason why each target is rebuilt.
#
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

#===========================================
# V - verbosity level
#===========================================
# 0 - quiet
# 1 - verbose
# 2 - silent
#===========================================
ifeq ("$(origin V)", "command line")
  PBUILD_VERBOSE = $(V)
endif
ifndef PBUILD_VERBOSE
  PBUILD_VERBOSE = 0
endif

ifeq ($(PBUILD_VERBOSE), 1)
  quiet =
  Q =
else
  quiet = quiet_
  Q = @
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands
ifneq ($(findstring s, $(filter-out --%, $(MAKEFLAGS))),)
  quiet = silent
  PBUILD_VERBOSE = 0
endif

export quiet Q PBUILD_VERBOSE

#===========================================
# O - output path
#===========================================
ifeq ("$(origin O)", "command line")
  PBUILD_OUTPUT := $(O)
endif

ifneq ($(PBUILD_OUTPUT),)
# Make's built-in functions such as $(abspath ...), $(realpath ...) cannot
# expand a shell special character '~'. We use a somewhat tedious way here
abs_objtree := $(shell mkdir -p $(PBUILD_OUTPUT) && cd $(PBUILD_OUTPUT) && pwd)
$(if $(abs_objtree),, \
	$(error failed to create output directory "$(PBUILD_OUTPUT)"))

# $(realpath ...) resolves symlinks
abs_objtree := $(realpath $(abs_objtree))
else
abs_objtree := $(CURDIR)
endif # ifneq ($(PBUILD_OUTPUT),)

ifeq ($(abs_objtree), $(CURDIR))
# Suppress "Entering directory ..." unless we are changind the work directory
MAKEFLAGS += --no-print-directory
else
need-sub-make :=1
endif

this-makefile := $(lastword $(MAKEFILE_LIST))
abs_srctree := $(realpath $(dir $(this-makefile)))

ifneq ($(abs_srctree), $(abs_objtree))
# Look for make include files relative to root of project src
#
# This does not become effective immediately because MAKEFLAGS is re-parsed
# once after the Makefile is read. We need to invoke sub-make.
MAKEFLAGS += --include-dir=$(abs_srctree)
need-sub-make := 1
endif

export abs_srctree abs_objtree
export sub_make_done := 1

#-------------------------------------------
# Invoking a second make in the output directory
#-------------------------------------------
ifeq ($(need-sub-make), 1)

PHONY += $(MAKECMDGOALS) __sub_make

$(filter-out $(this-makefile), $(MAKECMDGOALS)) __all: __sub-make
	@:
# Invoke a second make in the output directory, passing relevant variables
__sub-make:
	$(Q)$(MAKE) -C $(abs_objtree) -f $(abs_srctree)/Makefile $(MAKECMDGOALS)

endif # need-sub-make
endif # sub_make_done

# We process the rest of the Makefile if this is the final invocation of make
ifeq ($(need-sub-make),)

# Do not print "Entering directory ..."
# but we want to display it when entering to the output directory
MAKEFLAGS += --no-print-directory

ifeq ($(abs_srctree),$(abs_objtree))
	# building in the source tree
        srctree := .
        building_out_of_srctree :=
else
	ifeq ($(abs_srctree)/,$(dir $(abs_objtree)))
	# building in the directory of the source tree
                srctree := ..
	else
                srctree := $(abs_srctree)
	endif
	building_out_of_srctree := 1
endif

ifneq ($(PBUILD_ABS_SRCTREE),)
srctree := $(abs_srctree)
endif

objtree		:= .
VPATH		:= $(srctree)

export building_out_of_srctree srctree objtree VPATH

clean-targets	:= %clean mrproper cleandocs
#TODO dot-config
no-dot-config-targets := $(clean-targets) \
			 TAGS tags help% outputmakefile
no-compiler-targets := $(no-dot-config-targets)
single-targets	:= %.a %.o %/

mixed-build	:=
need-compiler	:= 1
single-build	:=

ifneq ($(filter $(no-compiler-targets), $(MAKECMDGOALS)),)
	ifeq ($(filter-out $(no-compiler-targets), $(MAKECMDGOALS)),)
		need-compiler :=
	endif
endif

# We cannot build single targets and the others at the same time
ifneq ($(filter $(single-targets), $(MAKECMDGOALS)),)
	single-build := 1
        ifneq ($(filter-out $(single-targets), $(MAKECMDGOALS)),)
		mixed-build := 1
        endif
endif

# For "make -j clean all", "make -j mrproper defconfig all", etc.
ifneq ($(filter $(clean-targets), $(MAKECMDGOALS)),)
        ifneq ($(filter-out $(clean-targets), $(MAKECMDGOALS)),)
		mixed-build := 1
        endif
endif

ifdef mixed-build
# ===========================================================================
# We're called with mixed targets (*config and build targets).
# Handle them one by one.

PHONY += $(MAKECMDGOALS) __build_one_by_one

$(MAKECMDGOALS): __build_one_by_one
	@:

__build_one_by_one:
	$(Q)set -e; \
	for i in $(MAKECMDGOALS); do \
		$(MAKE) -f $(srctree)/Makefile $$i; \
	done

else # !mixed-build

include $(srctree)/scripts/Pbuild.include

CONFIG_SHELL := sh

HOSTCC	= gcc
HOSTCXX	= g++

export PBUILD_USERCFLAGS := -Wall -Werror -Wmissing-prototypes -Wstrict-prototypes \
				-O2 -fomit-frame-pointer -std=gnu99
export PBUILD_USERLDFLAGS :=

PBUILD_HOSTCFLAGS   := $(PBUILD_USERCFLAGS) $(HOSTCFLAGS)
PBUILD_HOSTCXXFLAGS := -Wall -O2 $(HOSTCXXFLAGS)
PBUILD_HOSTLDFLAGS  := $(HOSTLDFLAGS)
PBUILD_HOSTLDLIBS   := $(HOSTLDLIBS)

# Make variables (CC, etc...)
CXX			= $(CROSS_COMPILE)g++
CC			= $(CROSS_COMPILE)gcc
LD			= $(CROSS_COMPILE)ld
AR			= $(CROSS_COMPILE)ar
NM			= $(CROSS_COMPILE)nm

YACC		= yacc
AWK			= awk
PERL		= perl
PYTHON3		= python3
BASH		= bash

PROJECTINCLUDE	:= \
		   -I$(srctree)/include \
		   -I$(objtree)/include \

PBUILD_AFLAGS	:=
PBUILD_CFLAGS	:= -Wall -Werror -Wundef -Werror=strict-prototypes -fPIE \
		   -std=gnu99 -g
PBUILD_CPPFLAGS	:=
PBUILD_LDFLAGS	:= -lGL -lglfw -ldl -lm -lpthread -lc -lassimp

export CONFIG_SHELL BASH HOSTCC PBUILD_HOSTCFLAGS CROSS_COMPILE LD CC CXX
export CPP AR YACC AWK PERL PYTHON3 MAKE HOSTCXX
export PBUILD_HOSTCXXFLAGS PBUILD_HOSTLDFLAGS PBUILD_HOSTLDLIBS

export PBUILD_CPPFLAGS PROJECTINCLUDE PBUILD_LDFLAGS
export PBUILD_CFLAGS
export PBUILD_AFLAGS

export RCS_FIND_IGNORE := \( -name SCCS -o -name BitKeeper -o -name .svn -o    \
			  -name CVS -o -name .pc -o -name .hg -o -name .git \) \
			  -prune -o
export RCS_TAR_IGNORE := --exclude SCCS --exclude BitKeeper --exclude .svn \
			 --exclude CVS --exclude .pc --exclude .hg --exclude .git

# ===========================================================================
# Rules shared between *config targets and build targets

# Basic helpers built in scripts/basic/
PHONY += scripts_basic
scripts_basic:
	$(Q)$(MAKE) $(build)=scripts/basic

PHONY += outputmakefile
ifdef building_out_of_srctree
# Before starting out-of-tree build, make sure the source tree is clean.
# outputmakefile generates a Makefile in the output directory, if using a
# separate output directory. This allows convenient use of make in the
# output directory.
# At the same time when output Makefile generated, generate .gitignore to
# ignore whole output directory

quiet_cmd_makefile = GEN     Makefile
      cmd_makefile ={ \
	echo "\# Automatically generated by $(srctree)/Makefile: don't edit"; \
	echo "include $(srctree)/Makefile"; \
	} > Makefile

outputmakefile:
#	$(Q)if [ -f $(srctree)/.config -o \
#		-d $(srctree)/include/config ]; then \
#		echo >&2 "***"; \
#		echo >&2 "*** The source tree is not clean, please run 'make mrproper'"; \
#		echo >&2 "*** in $(abs_srctree)"; \
#		echo >&2 "***"; \
#		false; \
#	fi
	$(Q)ln -fsn $(srctree) source
	$(call cmd, makefile)
	$(Q)test -e .gitignore || \
	{ echo "# this is a build directory, ignore it"; echo "*";} > .gitignore
endif

ifdef need-compiler
include $(srctree)/scripts/Makefile.compiler
endif

# ===========================================================================
# Build targets only
PHONY += all
__all: all

export PBUILD_BUILTIN := 1

# Objects we will link into main executable / subdirs we need to visit
core-y		:= engine/ src/
libs-y		:=

# The all: target is the default when no target is given on the command line.
# This allows a user to issue only 'make' to build the project
all: $(PBUILD_PROJECTNAME)

#TODO
PBUILD_CFLAGS	+= -O3

DEBUG_CFLAGS	:=
#TODO

# Add user supplied CPPFLAGS, AFLAGS and CFLAGS as the last assignments
PBUILD_CPPFLAGS += $(PCPPFLAGS)
PBUILD_AFLAGS += $(PAFLAGS)
PBUILD_CFLAGS += $(PCFLAGS)

PBUILD_USERCFLAGS += $(PBUILD_CFLAGS)
PBUILD_USERLDFLAGS += $(PBUILD_LDFLAGS)

PHONY += prepare0

core-y		+=

project-dirs	:= $(patsubst %/,%,$(filter %/, \
			$(core-y) $(libs-y)))
project-alldirs	:= $(sort $(project-dirs) Documentation \
			$(patsubst %/,%,$(filter %/, $(core-) \
			$(libs-))))

build-dirs	:= $(project-dirs)
clean-dirs	:= $(project-alldirs)

# Externally visible symbols (used by link-project.sh)
PBUILD_PROJECT_OBJS := $(head-y) $(patsubst %/,%/built-in.a, $(core-y))
PBUILD_PROJECT_OBJS += $(addsuffix built-in.a, $(filter %/, $(libs-y)))
PBUILD_PROJECT_LIBS := $(patsubst %/,%/lib.a, $(libs-y))

export PBUILD_PROJECT_OBJS PBUILD_PROJECT_LIBS
export PBUILD_LDS			:= scripts/project.lds

project-deps := $(PBUILD_PROJECT_OBJS) $(PBUILD_PROJECT_LIBS)

cmd_link-project =					\
	$(CONFIG_SHELL) $< "$(LD)" "$(PBUILD_LDFLAGS)"

$(PBUILD_PROJECTNAME): scripts/link-project.sh $(project-deps) FORCE
	+$(call if_changed_dep,link-project)

targets := $(PBUILD_PROJECTNAME)

# The actual objects are generated when descending,
# make sure no implicit rule kicks in
$(sort $(project-deps)): descend ;

# Additional helpers built in scripts/
# Carefully list dependencies so we do not try to build scripts twice
# in parallel
PHONY += scripts
scripts: scripts_basic
	$(Q)$(MAKE) $(build)=$(@)

PHONY += prepare

prepare0: scripts outputmakefile
	$(Q)$(MAKE) $(build)=.

prepare: prepare0

###
# Cleaning is done on three levels.
# make clean     Delete most generated files
#                Leave enough to build external modules
# make mrproper  Delete the current configuration, and all generated files
# make distclean Remove editor backup files, patch leftover files and the like

# Directories & files removed with 'make clean'
CLEAN_FILES +=

# Directories & files removed with 'make mrproper'
MRPROPER_FILES +=

# clean
#
clean: rm-files := $(CLEAN_FILES)

PHONY += projectclean

projectclean:
	$(Q)$(CONFIG_SHELL) $(srctree)/scripts/link-project.sh clean

clean: projectclean

# mrproper - Delete all generated files
#
mrproper: rm-files := $(wildcard $(MRPROPER_FILES))

mrproper-dirs      := $(addprefix _mrproper_,scripts)

PHONY += $(mrproper-dirs) mrproper
$(mrproper-dirs):
	$(Q)$(MAKE) $(clean)=$(patsubst _mrproper_%,%,$@)

mrproper: clean $(mrproper-dirs)
	$(call cmd,rmfiles)

# distclean
#
PHONY += distclean

distclean: mrproper
	@find . $(RCS_FIND_IGNORE) \
		\( -name '*.orig' -o -name '*.rej' -o -name '*~' \
		-o -name '*.bak' -o -name '#*#' -o -name '*%' \
		-o -name 'core' -o -name tags -o -name TAGS -o -name 'cscope*' \
		-o -name GPATH -o -name GRTAGS -o -name GSYMS -o -name GTAGS \) \
		-type f -print | xargs rm -f

# Brief documentation of the typical targets used
# ---------------------------------------------------------------------------
#TODO

ifdef single-build

single-no-ko := $(sort $(patsubst %.ko,%.mod, $(MAKECMDGOALS)))

$(single-no-ko): descend
	@:

export PBUILD_SINGLE_TARGETS := $(single-no-ko)

endif

# Handle descending into subdirectories listed in $(build-dirs)
# Preset locale variables to speed up the build process. Limit locale
# tweaks to this spot to avoid wrong language settings when running
# make menuconfig etc.
# Error messages still appears in the original language
PHONY += descend $(build-dirs)
descend: $(build-dirs)
$(build-dirs): prepare
	$(Q)$(MAKE) $(build)=$@ \
	single-build=$(if $(filter-out $@/, $(filter $@/%, $(PBUILD_SINGLE_TARGETS))),1) \
	need-builtin=1

clean-dirs := $(addprefix _clean_, $(clean-dirs))
PHONY += $(clean-dirs) clean
$(clean-dirs):
	$(Q)$(MAKE) $(clean)=$(patsubst _clean_%,%,$@)

clean: $(clean-dirs)
	$(call cmd,rmfiles)
	@find . $(RCS_FIND_IGNORE) \
		\( -name '*.[aios]' -o -name '*.ko' -o -name '.*.cmd' \
		-o -name '*.ko.*' \
		-o -name '*.dtb' -o -name '*.dtbo' -o -name '*.dtb.S' -o -name '*.dt.yaml' \
		-o -name '*.dwo' -o -name '*.lst' \
		-o -name '*.su' -o -name '*.mod' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \
		-o -name '*.lex.c' -o -name '*.tab.[ch]' \
		-o -name '*.asn1.[ch]' \
		-o -name '*.symtypes' -o -name 'modules.order' \
		-o -name '.tmp_*.o.*' \
		-o -name '*.c.[012]*.*' \
		-o -name '*.ll' \
		-o -name '*.gcno' \
		-o -name '*.*.symversions' \) -type f -print | xargs rm -f

# Generate tags for editors
# ---------------------------------------------------------------------------
quiet_cmd_tags = GEN     $@
      cmd_tags = $(BASH) $(srctree)/scripts/tags.sh $@

tags TAGS cscope gtags: FORCE
	$(call cmd,tags)

# Run valgrind to test for memory leaks
# ---------------------------------------------------------------------------
quiet_cmd_test = TEST    $<
      cmd_test = valgrind $(abs_objtree)/$<

test: $(PBUILD_PROJECTNAME) FORCE
	$(call cmd,test)

# Run gdb
# ---------------------------------------------------------------------------
quiet_cmd_debug = DEBUG   $<
      cmd_debug = gdb $(abs_objtree)/$<

debug: $(PBUILD_PROJECTNAME) FORCE
	$(call cmd,debug)

# Run program
# ---------------------------------------------------------------------------
quiet_cmd_run = RUN     $<
      cmd_run = $(abs_objtree)/$<

run: $(PBUILD_PROJECTNAME) FORCE
	$(call cmd,run)

# read saved command lines for existing targets
existing-targets := $(wildcard $(sort $(targets)))

-include $(foreach f,$(existing-targets),$(dir $(f)).$(notdir $(f)).cmd)

endif # mixed-build
endif # need-sub-make

PHONY += FORCE
FORCE:

# Declare the contents of the PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)
