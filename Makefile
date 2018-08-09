#
# RMANTREE points to the RenderManProServer directory.
# Set it if it's not already set in the environment.
#
RMANTREE ?= /opt/pixar/RenderManProServer-22.0

#
# BUILD_DIR is where the built files will go.
# Set it if it's not already set in the environment.
#
SRC_DIR ?= $(CURDIR)
BUILD_DIR ?= $(SRC_DIR)/build

#
# Base command for copying files to the build directory.
#
COPY = rsync -rum --include "*/"

#
# Control variables.
# These subdirectories will be built using their own Makefile.
#
SUBDIRS := osl ris

#
# Targets and prerequisites.
# clean is the list of (phony) targets for cleaning the SUBDIRS directory contents.
# install contains the (phony) targets for installing the SUBDIRS directory contents.
#
clean := $(patsubst %,%.clean,$(SUBDIRS))
install := $(patsubst %,%.install,$(SUBDIRS))

.PHONY : $(SUBDIRS) build

build : $(SUBDIRS) install

#
# Rules.
#
clean : $(clean)
	-$(shell rm -rf $(BUILD_DIR))

%.clean :
	$(MAKE) -C $(subst .clean,,$@) clean

$(SUBDIRS) :
	$(MAKE) -C $@

install : $(install)
	-$(shell mkdir -p $(BUILD_DIR))
	-$(COPY) --include="*.katana" --exclude="*" scene $(BUILD_DIR)
	-$(COPY) --include="*.macro" --exclude="*" katana $(BUILD_DIR)

%.install : $(SUBDIRS)
	-$(COPY) --include="*.oso" --include="*.so" --include="*.args" --exclude="*" $(subst .install,,$@) $(BUILD_DIR)
