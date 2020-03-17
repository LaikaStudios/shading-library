#
# RMANTREE points to the RenderManProServer directory.
# Set it if it's not already set in the environment.
#
RMANTREE ?= /opt/pixar/RenderManProServer-23.2

#
# Compiler and linker settings.
#
CC = clang++
LD = clang++ -shared
CFLAGS = -Wall -Wno-switch -Wno-reorder -Wno-sign-compare -Wno-narrowing -Wno-deprecated-declarations -fPIC -std=c++14 -O3
INCLUDE = -I. -I./include -I${RMANTREE}/include

#
# Control variables.
# These subdirectories will be searched for .cpp files, which will then be built into .so files.
#
SUBDIRS := displace

# These shaders won't be built. Use 'subdirectory/basename'.
EXCLUDE := 

#
# Make functionality.
# src will be the list of .cpp files to build from.
#
src := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))

EXCLUDE := $(basename $(EXCLUDE))
EXCLUDE := $(addsuffix .cpp, $(EXCLUDE))

src := $(filter-out $(EXCLUDE), $(src))

#
# Create target lists.
#
obj := $(src:%.cpp=%.o)
dso := $(src:%.cpp=%.so)

#
# Targets and prerequisites.
#
all: $(dso)

$(obj): ${RMANTREE}/include/*.h

#
# Rules.
#
clean:
	-$(shell rm -f $(shell find . -name "*.so" -print) $(shell find . -name "*.o" -print))

show_src:
	@echo $(src)

show_obj:
	@echo $(obj)

show_dso:
	@echo $(dso)

#
# Build rules.
#
%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.so : %.o
	$(LD) -o $@ $<
