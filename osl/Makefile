#
# RMANTREE points to the RenderManProServer directory.
# Set it if it's not already set in the environment.
#
RMANTREE ?= /opt/pixar/RenderManProServer-22.0

#
# Compiler settings.
#
CC = ${RMANTREE}/bin/oslc
CFLAGS = 
INCLUDE = -I. -I./include

#
# Control variables.
# These subdirectories will be searched for .osl files, which will then be built into .oso files.
#
SUBDIRS := material

# These shaders won't be built. Use 'subdirectory/basename'.
EXCLUDE := 

#
# Make functionality.
# src will be the list of .osl files to build from.
# obj will be the list of .oso files to build.
#
src := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.osl))

EXCLUDE := $(basename $(EXCLUDE))
EXCLUDE := $(addsuffix .osl, $(EXCLUDE))

src := $(filter-out $(EXCLUDE), $(src))
obj := $(src:%.osl=%.oso)

#
# Targets and prerequisites.
#
all: $(obj)

#
# Rules.
#
clean:
	-$(shell rm -f $(shell find . -name "*.oso" -print))

show_src:
	@echo $(src)

show_obj:
	@echo $(obj)

#
# Build rules.
#
%.oso : %.osl
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<
