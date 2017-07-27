# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := $(BUILDDIR)/lib
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/deps
EXE := $(BINDIR)/libnas2d.a

CFLAGS := -std=c++11 -g -Wall -I$(INCDIR) -I$(ADDITIONAL_INCLUDES) $(shell sdl2-config --cflags)
LDFLAGS := -lstdc++ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLU -lGL

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
FOLDERS := $(sort $(dir $(SRCS)))

all: $(EXE)

$(EXE): $(OBJS)
	@mkdir -p ${@D}
	ar rcs $@ $^

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(DEPDIR)/%.d | build-folder
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

.PHONY:build-folder
build-folder:
	@mkdir -p $(patsubst $(SRCDIR)/%,$(OBJDIR)/%, $(FOLDERS))
	@mkdir -p $(patsubst $(SRCDIR)/%,$(DEPDIR)/%, $(FOLDERS))

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))

.PHONY:clean
clean:
	-rm -fr $(BUILDDIR)

.PHONY:clean-deps
clean-deps:
	-rm -fr $(DEPDIR)

# vim: filetype=make



### Linux development package dependencies ###
# This section contains install rules to aid setup and compiling on Linux.
# Only a few common Linux distributions are covered. Other distributions
# should be similar.


## Arch Linux ##

.PHONY:install-deps-arch
install-deps-arch:
	pacman -S sdl2 sdl2_mixer sdl2_image sdl2_ttf glew physfs


## Ubuntu ##

.PHONY:install-deps-ubuntu
install-deps-ubuntu:
	apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev libphysfs-dev


## CentOS ##

.PHONY:install-repos-centos
install-repos-centos:
	# Default CentOS repositories only contain SDL1
	# For SDL2 use EPEL repo (EPEL = Extra Packages for Enterprise Linux)
	yum install epel-release

.PHONY:install-deps-centos
install-deps-centos:
	# Install development packages (-y answers "yes" to prompts)
	yum -y install SDL2-devel SDL2_mixer-devel SDL2_image-devel SDL2_ttf-devel glew-devel physfs-devel


## Generic SDL2 source build ##

SdlVer := SDL2-2.0.5
SdlArchive := $(SdlVer).tar.gz
SdlUrl := "https://www.libsdl.org/release/$(SdlArchive)"
SdlPackageDir := $(BUILDDIR)/sdl2
SdlDir := $(SdlPackageDir)/$(SdlVer)

.PHONY:install-deps-source-sdl2
install-deps-source-sdl2:
	# Create source build folder
	mkdir -p $(SdlDir)
	# Download source archive
	wget --no-clobber --directory-prefix=$(SdlPackageDir) $(SdlUrl)
	# Unpack archive
	cd $(SdlPackageDir) && tar -xzf $(SdlArchive)
	# Configure package
	cd $(SdlDir) && ./configure --quiet --enable-mir-shared=no
	# Compile package
	cd $(SdlDir) && make

