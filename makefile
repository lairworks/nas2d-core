# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

# Capture top level folder before any Makefile includes
# Note: MAKEFILE_LIST's last entry is the last processed Makefile.
#       That should be the current Makefile, assuming no includes
TopLevelFolder := $(abspath $(dir $(lastword ${MAKEFILE_LIST})))

SRCDIR := src
INCDIR := include
BUILDDIR := .build
BINDIR := lib
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/deps
OUTPUT := $(BINDIR)/libnas2d.a

# Determine OS (Linux, Darwin, ...)
CURRENT_OS := $(shell uname 2>/dev/null || echo Unknown)
TARGET_OS ?= $(CURRENT_OS)

Linux_OpenGL_LIBS := -lGLEW -lGL
Darwin_OpenGL_LIBS := -lGLEW -framework OpenGL
Windows_OpenGL_LIBS := -lglew32 -lopengl32
OpenGL_LIBS := $($(TARGET_OS)_OpenGL_LIBS)

CPPFLAGS := $(CPPFLAGS.EXTRA) -Iinclude/
CXXFLAGS := -std=c++17 -g -Wall -Wpedantic $(shell sdl2-config --cflags)
LDFLAGS := $(LDFLAGS.EXTRA)
LDLIBS := -lstdc++ -lphysfs -lSDL2_image -lSDL2_mixer -lSDL2_ttf $(shell sdl2-config --static-libs) $(OpenGL_LIBS)

Windows_RUN_PREFIX := wine
RUN_PREFIX := $($(TARGET_OS)_RUN_PREFIX)

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
FOLDERS := $(sort $(dir $(SRCS)))

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	@mkdir -p ${@D}
	ar rcs $@ $^

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(DEPDIR)/%.d | build-folder
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

.PHONY: build-folder
build-folder:
	@mkdir -p $(patsubst $(SRCDIR)/%,$(OBJDIR)/%, $(FOLDERS))
	@mkdir -p $(patsubst $(SRCDIR)/%,$(DEPDIR)/%, $(FOLDERS))

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SRCS)))

.PHONY: clean clean-deps clean-all
clean:
	-rm -fr $(OBJDIR)
	-rm -fr $(DEPDIR)
	-rm -fr $(BINDIR)
clean-deps:
	-rm -fr $(DEPDIR)
clean-all: | clean
	-rm -rf $(BUILDDIR)


## Unit Test project ##

.PHONY: gtest gmock test check

# Either of these should be a complete combined package. Only build one.
GTESTSRCDIR := /usr/src/gtest/
GMOCKSRCDIR := /usr/src/gmock/
GTESTDIR := $(BUILDDIR)/gtest
GMOCKDIR := $(BUILDDIR)/gmock

gtest:
	mkdir -p $(GTESTDIR)
	cd $(GTESTDIR) && cmake -DCMAKE_CXX="$(CXX)" -DCMAKE_CXX_FLAGS="-std=c++17" $(GTESTSRCDIR)
	make -C $(GTESTDIR)

gmock:
	mkdir -p $(GMOCKDIR)
	cd $(GMOCKDIR) && cmake -DCMAKE_CXX="$(CXX)" -DCMAKE_CXX_FLAGS="-std=c++17" $(GMOCKSRCDIR)
	make -C $(GMOCKDIR)

TESTDIR := test
TESTOBJDIR := $(BUILDDIR)/testObj
TESTSRCS := $(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.o,$(TESTSRCS))
TESTFOLDERS := $(sort $(dir $(TESTSRCS)))
TESTCPPFLAGS := $(CPPFLAGS)
TESTLDFLAGS := -L$(BINDIR) $(LDFLAGS)
TESTLIBS := -lnas2d -lgtest -lgtest_main -lgmock -lgmock_main -lpthread $(LDLIBS)
TESTOUTPUT := $(BUILDDIR)/testBin/runTests

TESTDEPFLAGS = -MT $@ -MMD -MP -MF $(TESTOBJDIR)/$*.Td
TESTCOMPILE.cpp = $(CXX) $(TESTCPPFLAGS) $(TESTDEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
TESTPOSTCOMPILE = @mv -f $(TESTOBJDIR)/$*.Td $(TESTOBJDIR)/$*.d && touch $@

test: $(TESTOUTPUT)
check: | test
	cd test && $(RUN_PREFIX) ../$(TESTOUTPUT)

$(TESTOUTPUT): $(TESTOBJS) $(OUTPUT)
	@mkdir -p ${@D}
	$(CXX) $(TESTOBJS) $(TESTLDFLAGS) $(TESTLIBS) -o $@

$(TESTOBJS): $(TESTOBJDIR)/%.o : $(TESTDIR)/%.cpp $(TESTOBJDIR)/%.d | test-build-folder
	$(TESTCOMPILE.cpp) $(OUTPUT_OPTION) -I$(SRCDIR) $<
	$(TESTPOSTCOMPILE)

.PHONY: test-build-folder
test-build-folder:
	@mkdir -p $(patsubst $(TESTDIR)/%,$(TESTOBJDIR)/%, $(TESTFOLDERS))

$(TESTOBJDIR)/%.d: ;
.PRECIOUS: $(TESTOBJDIR)/%.d

include $(wildcard $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.d,$(TESTSRCS)))


### Linux development package dependencies ###
# This section contains install rules to aid setup and compiling on Linux.
# Only a few common Linux distributions are covered. Other distributions
# should be similar.


## Arch Linux ##

.PHONY: install-deps-arch
install-deps-arch:
	pacman -S sdl2 sdl2_mixer sdl2_image sdl2_ttf glew physfs


## Ubuntu ##

.PHONY: install-deps-ubuntu
install-deps-ubuntu:
	apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev libphysfs-dev


## CentOS ##

.PHONY: install-repos-centos
install-repos-centos:
	# Default CentOS repositories only contain SDL1
	# For SDL2 use EPEL repo (EPEL = Extra Packages for Enterprise Linux)
	yum install epel-release

.PHONY: install-deps-centos
install-deps-centos:
	# Install development packages (-y answers "yes" to prompts)
	yum -y install SDL2-devel SDL2_mixer-devel SDL2_image-devel SDL2_ttf-devel glew-devel physfs-devel


#### Docker related build rules ####

# Build rules relating to Docker images
.PHONY: build-image-ubuntu compile-on-ubuntu
.PHONY: debug-image-ubuntu root-debug-image-ubuntu

DockerFolder := ${TopLevelFolder}/docker
DockerRunFlags := --volume ${TopLevelFolder}:/code
DockerRepository := outpostuniverse

ImageName := nas2d
ImageVersion := 1.3

build-image-ubuntu:
	docker build ${DockerFolder}/ --file ${DockerFolder}/${ImageName}.Dockerfile --tag ${DockerRepository}/${ImageName}:latest --tag ${DockerRepository}/${ImageName}:${ImageVersion}
compile-on-ubuntu:
	docker run ${DockerRunFlags} --rm --tty ${DockerRepository}/${ImageName}
debug-image-ubuntu:
	docker run ${DockerRunFlags} --rm --tty --interactive ${DockerRepository}/${ImageName} bash
root-debug-image-ubuntu:
	docker run ${DockerRunFlags} --rm --tty --interactive --user=0 ${DockerRepository}/${ImageName} bash

#### CircleCI related build rules ####

.PHONY: build-image-circleci push-image-circleci circleci-validate circleci-build

build-image-circleci: | build-image-ubuntu
	docker build .circleci/ --tag outpostuniverse/nas2d-circleci:latest --tag outpostuniverse/nas2d-circleci:1.3
push-image-circleci:
	docker push outpostuniverse/nas2d-circleci
circleci-validate:
	circleci config validate
circleci-build:
	circleci build
