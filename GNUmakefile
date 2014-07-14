# -*- Makefile -*-

#
# step #1 - system specific compilers, setttings etc
#

OSTYPE := $(shell uname -s)

#
#
#

ifeq ($(CONFIG),) # not defined

include build/GNUmakefile.config # default is debug

endif


#
# external libraries
#

LOCAL_INCLUDE =

#
# utilities
#

ECHO := /bin/echo

#
# defines
#

DEFS  :=

#
# compiler flags, depnLdncy generation and implicit build rules
#

-include build/GNUmakefile.compiler
-include build/GNUmakefile.ABI
-include build/GNUmakefile.depends
-include build/GNUmakefile.rules

#
# sources, objects and targets
#
# object files - done by string substitution
#

SRCS_LCG := main.cpp LCG_PLE63.cpp
OBJS_LCG := $(addsuffix .o, $(basename $(SRCS_LCG)))	
DEPS_LCG := $(addsuffix .d, $(basename $(SRCS_LCG)))

OBJS := $(OBJS_LCG)
DEPS := $(DEPS_LCG)

# all targets

LCG := lcg

.PHONY: all tags dox clean realclean

all: $(LCG)

$(LCG): $(OBJS_LCG)
	$(LD) $(LDFLAGS) -o $@ $(wordlist 1,10000000,$^) -L/usr/local/lib $(SYSLIB)	

#dependencies
-include $(DEPS)

tags:
	@(find . -name "*.h" -print | etags -)
	@(find . -name "*.cpp" -print | etags -a -)
	@(find . -name "*.cc" -print | etags -a -)
	@/bin/echo "TAGS updated"

dox:
	doxygen

clean:
	-rm -rf $(LCG)
	-rm -rf $(OBJS)

realclean:
	-rm -rf $(LCG)
	-rm -rf $(OBJS)
	-rm -rf $(DEPS)
	-rm -rf TAGS
