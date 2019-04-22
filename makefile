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

# SDL2 source build variables
SdlVer := SDL2-2.0.5
SdlArchive := $(SdlVer).tar.gz
SdlUrl := "https://www.libsdl.org/release/$(SdlArchive)"
SdlPackageDir := $(BUILDDIR)/sdl2
SdlDir := $(SdlPackageDir)/$(SdlVer)
# Include folder for newer source build
# (Must be searched before system folder returned by sdl2-config)
SdlInc := $(SdlDir)/include

CXXFLAGS := -std=c++14 -g -Wall -I$(INCDIR) -I$(SdlInc) $(shell sdl2-config --cflags)
LDLIBS := -lstdc++ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLU -lGL

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
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

.PHONY: clean clean-deps clean-sdl clean-sdl-all clean-all
clean:
	-rm -fr $(OBJDIR)
	-rm -fr $(DEPDIR)
	-rm -fr $(BINDIR)
clean-deps:
	-rm -fr $(DEPDIR)
clean-sdl:
	-rm -fr $(SdlDir)
clean-sdl-all:
	-rm -fr $(SdlPackageDir)
clean-all:
	-rm -rf $(BUILDDIR)


## Unit Test project ##

.PHONY: gtest gmock check

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

# This is used to detect if a separate GMock library was built, in which case, use it
GMOCKLIB := $(wildcard $(GMOCKDIR)/libgmock.a)

TESTDIR := test
TESTOBJDIR := $(BUILDDIR)/testObj
TESTSRCS := $(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.o,$(TESTSRCS))
TESTFOLDERS := $(sort $(dir $(TESTSRCS)))
TESTCPPFLAGS := -I$(INCDIR) -I$(GMOCKSRCDIR)/gtest/include
TESTLDFLAGS := -L$(BINDIR) -L$(GMOCKDIR) -L$(GMOCKDIR)/gtest/ -L$(GTESTDIR)
TESTLIBS := -lnas2d -lgtest -lgtest_main -lpthread -lstdc++fs $(LDLIBS)
TESTOUTPUT := $(BUILDDIR)/testBin/runTests
# Conditionally add GMock if we built it separately
# This is conditional to avoid errors in case the library is not found
ifneq ($(strip $(GMOCKLIB)),)
	TESTLIBS := -lgmock $(TESTLIBS)
endif

TESTDEPFLAGS = -MT $@ -MMD -MP -MF $(TESTOBJDIR)/$*.Td
TESTCOMPILE.cpp = $(CXX) $(TESTCPPFLAGS) $(TESTDEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
TESTPOSTCOMPILE = @mv -f $(TESTOBJDIR)/$*.Td $(TESTOBJDIR)/$*.d && touch $@

check: $(TESTOUTPUT)
	cd test && ../$(TESTOUTPUT)

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


## Generic SDL2 source build ##

.PHONY: install-deps-source-sdl2
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


#### Docker related build rules ####

# Build rules relating to Docker images
.PHONY: build-image-ubuntu-16.04 compile-on-ubuntu-16.04
.PHONY: debug-image-ubuntu-16.04 root-debug-image-ubuntu-16.04

.PHONY: build-image-ubuntu-18.04 compile-on-ubuntu-18.04
.PHONY: debug-image-ubuntu-18.04 root-debug-image-ubuntu-18.04

DockerFolder := ${TopLevelFolder}/docker

build-image-ubuntu-16.04:
	docker build ${DockerFolder}/ --file ${DockerFolder}/Ubuntu-16.04.BuildEnv.Dockerfile --tag outpostuniverse/ubuntu-16.04-gcc-sdl2-physfs
compile-on-ubuntu-16.04:
	docker run --rm --tty --volume ${TopLevelFolder}:/code outpostuniverse/ubuntu-16.04-gcc-sdl2-physfs
debug-image-ubuntu-16.04:
	docker run --rm --tty --volume ${TopLevelFolder}:/code --interactive outpostuniverse/ubuntu-16.04-gcc-sdl2-physfs bash
root-debug-image-ubuntu-16.04:
	docker run --rm --tty --volume ${TopLevelFolder}:/code --interactive --user=0 outpostuniverse/ubuntu-16.04-gcc-sdl2-physfs bash

build-image-ubuntu-18.04:
	docker build ${DockerFolder}/ --file ${DockerFolder}/Ubuntu-18.04-gcc.Dockerfile --tag outpostuniverse/ubuntu-18.04-gcc:latest --tag outpostuniverse/ubuntu-18.04-gcc:1.0
	docker build ${DockerFolder}/ --file ${DockerFolder}/Ubuntu-18.04-gcc-gtest.Dockerfile --tag outpostuniverse/ubuntu-18.04-gcc-gtest:latest --tag outpostuniverse/ubuntu-18.04-gcc-gtest:1.1
	docker build ${DockerFolder}/ --file ${DockerFolder}/nas2d.Dockerfile --tag outpostuniverse/nas2d:latest --tag outpostuniverse/nas2d:1.1
compile-on-ubuntu-18.04:
	docker run --rm --tty --volume ${TopLevelFolder}:/code outpostuniverse/nas2d
debug-image-ubuntu-18.04:
	docker run --rm --tty --volume ${TopLevelFolder}:/code --interactive outpostuniverse/nas2d bash
root-debug-image-ubuntu-18.04:
	docker run --rm --tty --volume ${TopLevelFolder}:/code --interactive --user=0 outpostuniverse/nas2d bash


#### CircleCI related build rules ####

.PHONY: build-image-circleci push-image-circleci circleci-validate circleci-build

build-image-circleci: | build-image-ubuntu-18.04
	docker build .circleci/ --tag outpostuniverse/nas2d-circleci:latest --tag outpostuniverse/nas2d-circleci:1.1
push-image-circleci:
	docker push outpostuniverse/nas2d-circleci
circleci-validate:
	circleci config validate
circleci-build:
	circleci build
