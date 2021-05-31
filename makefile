# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

# Capture top level folder before any Makefile includes
# Note: MAKEFILE_LIST's last entry is the last processed Makefile.
#       That should be the current Makefile, assuming no includes
TopLevelFolder := $(abspath $(dir $(lastword ${MAKEFILE_LIST})))

# Determine OS (Linux, Darwin, ...)
CURRENT_OS := $(shell uname 2>/dev/null || echo Unknown)
TARGET_OS ?= $(CURRENT_OS)

Linux_OpenGL_LIBS := -lGLEW -lGL
Darwin_OpenGL_LIBS := -lGLEW -framework OpenGL
Windows_OpenGL_LIBS := -lglew32 -lopengl32
OpenGL_LIBS := $($(TARGET_OS)_OpenGL_LIBS)

SDL_CONFIG_CFLAGS := $(shell type sdl2-config >/dev/null 2>&1 && sdl2-config --cflags)
SDL_CONFIG_LIBS := $(shell type sdl2-config >/dev/null 2>&1 && sdl2-config --static-libs)

CPPFLAGS := $(CPPFLAGS_EXTRA)
CXXFLAGS_WARN := -Wall -Wextra -Wpedantic -Wzero-as-null-pointer-constant -Wnull-dereference -Wold-style-cast -Wcast-qual -Wcast-align -Wdouble-promotion -Wshadow -Wnon-virtual-dtor -Woverloaded-virtual -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Winvalid-pch -Wmissing-format-attribute -Wredundant-decls $(WARN_EXTRA)
CXXFLAGS := $(CXXFLAGS_EXTRA) -std=c++17 $(CXXFLAGS_WARN) $(SDL_CONFIG_CFLAGS)
LDFLAGS := $(LDFLAGS_EXTRA)
LDLIBS := $(LDLIBS_EXTRA) -lstdc++ -lphysfs -lSDL2_image -lSDL2_mixer -lSDL2_ttf $(SDL_CONFIG_LIBS) $(OpenGL_LIBS)

Windows_RUN_PREFIX := wine
RUN_PREFIX := $($(TARGET_OS)_RUN_PREFIX)

SRCDIR := NAS2D
BUILDDIR := .build
BINDIR := lib
INTDIR := $(BUILDDIR)/intermediate
OUTPUT := $(BINDIR)/libnas2d.a
PACKAGEDIR := $(BUILDDIR)/package

DEPFLAGS = -MT $@ -MMD -MP -MF $(INTDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(INTDIR)/$*.Td $(INTDIR)/$*.d && touch $@

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.o,$(SRCS))
FOLDERS := $(sort $(dir $(SRCS)))

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	@mkdir -p "${@D}"
	ar rcs $@ $^

$(OBJS): $(INTDIR)/%.o : $(SRCDIR)/%.cpp $(INTDIR)/%.d
	@mkdir -p "${@D}"
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(INTDIR)/%.d: ;
.PRECIOUS: $(INTDIR)/%.d

include $(wildcard $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.d,$(SRCS)))


VERSION = $(shell git describe --tags --dirty)
CONFIG = $(TARGET_OS).x64
PACKAGE_NAME = $(PACKAGEDIR)/nas2d-$(VERSION)-$(CONFIG).tar.gz
Darwin_TAR_RENAME_FLAG := -s '!^$(SRCDIR)/!include/\0!'
Linux_TAR_RENAME_FLAG := --transform='s/^$(SRCDIR)/include\/\0/'
TAR_RENAME_FLAG := $($(CURRENT_OS)_TAR_RENAME_FLAG)

.PHONY: package
package: $(PACKAGE_NAME)

$(PACKAGE_NAME): $(OUTPUT) $(shell find $(SRCDIR) -name '*.h')
	@mkdir -p "$(PACKAGEDIR)"
	# Package an "include/" folder containing all header files, plus the library file
	find $(SRCDIR) -name '*.h' | tar -czf $(PACKAGE_NAME) $(TAR_RENAME_FLAG) -T - $(OUTPUT)


.PHONY: clean clean-all
clean:
	-rm -fr $(INTDIR)
clean-all: | clean
	-rm -rf $(BUILDDIR)
	-rm -fr $(BINDIR)


## Unit Test project ##

# Either of these should be a complete combined package. Only build one.
GTESTSRCDIR := /usr/src/googletest/
GTESTDIR := $(BUILDDIR)/gtest

.PHONY: gtest
gtest:
	mkdir -p $(GTESTDIR)
	cd $(GTESTDIR) && \
	  curl --location https://github.com/google/googletest/archive/release-1.10.0.tar.gz | tar -xz && \
	  cmake -DCMAKE_CXX_FLAGS="-std=c++17" googletest-release-1.10.0/ && \
	  make && \
	  cmake -DCMAKE_CXX_FLAGS="-std=c++17" -DBUILD_SHARED_LIBS=ON googletest-release-1.10.0/ && \
	  make

.PHONY: gtest-install
gtest-install:
	cd $(GTESTDIR) && \
	  cp -r lib/ /usr/local/ && \
	  cp -r \
	    googletest-release-1.10.0/googletest/include/ \
	    googletest-release-1.10.0/googlemock/include/ \
	    /usr/local/ && \
	  cp --parents -r \
	    googletest-release-1.10.0/CMakeLists.txt \
	    googletest-release-1.10.0/googletest/CMakeLists.txt \
	    googletest-release-1.10.0/googletest/cmake/ \
	    googletest-release-1.10.0/googletest/src/ \
	    googletest-release-1.10.0/googlemock/CMakeLists.txt \
	    googletest-release-1.10.0/googlemock/cmake/ \
	    googletest-release-1.10.0/googlemock/src/ \
	    /usr/local/src/

TESTDIR := test
TESTINTDIR := $(BUILDDIR)/testIntermediate
TESTSRCS := $(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTINTDIR)/%.o,$(TESTSRCS))
TESTFOLDERS := $(sort $(dir $(TESTSRCS)))
TESTCPPFLAGS := $(CPPFLAGS) -I./
TESTLDFLAGS := -L$(BINDIR) $(LDFLAGS)
TESTLIBS := -lnas2d -lgtest -lgtest_main -lgmock -lgmock_main -lpthread $(LDLIBS)
TESTOUTPUT := $(BUILDDIR)/testBin/runTests

TESTDEPFLAGS = -MT $@ -MMD -MP -MF $(TESTINTDIR)/$*.Td
TESTCOMPILE.cpp = $(CXX) $(TESTCPPFLAGS) $(TESTDEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
TESTPOSTCOMPILE = @mv -f $(TESTINTDIR)/$*.Td $(TESTINTDIR)/$*.d && touch $@

.PHONY: test
test: $(TESTOUTPUT)

.PHONY: check
check: | test
	cd test && $(RUN_PREFIX) ../$(TESTOUTPUT)

$(TESTOUTPUT): $(TESTOBJS) $(OUTPUT)
	@mkdir -p "${@D}"
	$(CXX) $(TESTOBJS) $(TESTLDFLAGS) $(TESTLIBS) -o $@

$(TESTOBJS): $(TESTINTDIR)/%.o : $(TESTDIR)/%.cpp $(TESTINTDIR)/%.d
	@mkdir -p "${@D}"
	$(TESTCOMPILE.cpp) $(OUTPUT_OPTION) -I$(SRCDIR) $<
	$(TESTPOSTCOMPILE)

$(TESTINTDIR)/%.d: ;
.PRECIOUS: $(TESTINTDIR)/%.d

include $(wildcard $(patsubst $(TESTDIR)/%.cpp,$(TESTINTDIR)/%.d,$(TESTSRCS)))


.PHONY: test-graphics
test-graphics: $(BUILDDIR)/testGraphics
$(BUILDDIR)/testGraphics: $(OUTPUT)
	@mkdir -p $(BUILDDIR)
	$(CXX) -o $(BUILDDIR)/testGraphics test-graphics/*.cpp $(TESTCPPFLAGS) $(CXXFLAGS) $(TESTLDFLAGS) -lnas2d $(LDLIBS)

.PHONY: run-test-graphics
run-test-graphics: | test-graphics
	cd test-graphics/ && ../.build/testGraphics ; cd ..


.PHONY: lint
lint: cppcheck cppclean

.PHONY: cppcheck
cppcheck:
	cppcheck --quiet "$(SRCDIR)"

.PHONY: cppclean
cppclean:
	cppclean "$(SRCDIR)"

### Linux development package dependencies ###
# This section contains install rules to aid setup and compiling on Linux.
# Only a few common Linux distributions are covered. Other distributions
# should be similar.

RELEASE_FILES = $(wildcard /etc/*-release)
RELEASE_FILE_TAGS = $(filter-out os lsb,$(patsubst /etc/%-release,%,$(RELEASE_FILES)))
RELEASE_SETTING_NAME = $(shell '$(SHELL)' -c '[ -f /etc/os-release ] && . /etc/os-release && echo $${ID}')
LINUX_DISTRIBUTION = $(or $(RELEASE_SETTING_NAME),$(RELEASE_FILE_TAGS),Unknown)
DISTRIBUTION = $(subst Darwin,darwin,$(subst Linux,$(LINUX_DISTRIBUTION),$(CURRENT_OS)))

.PHONY: install-dependencies
install-dependencies:
	@echo
	@echo "Detected distribution: $(DISTRIBUTION)"
	@echo
	$(MAKE) "install-dependencies-$(DISTRIBUTION)"

## Ubuntu ##
.PHONY: install-dependencies-ubuntu
install-dependencies-ubuntu:
	apt --yes install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev libphysfs-dev

## CentOS ##
.PHONY: install-dependencies-centos
install-dependencies-centos: | install-dependencies-repository-centos
	# Install development packages (-y answers "yes" to prompts)
	yum --assumeyes install SDL2-devel SDL2_mixer-devel SDL2_image-devel SDL2_ttf-devel glew-devel physfs-devel
.PHONY: install-dependencies-repository-centos
install-dependencies-repository-centos:
	# Default CentOS repositories only contain SDL1
	# For SDL2 use EPEL repo (EPEL = Extra Packages for Enterprise Linux)
	yum --assumeyes install epel-release

## Arch Linux ##
.PHONY: install-dependencies-arch
install-dependencies-arch:
	pacman --sync --refresh sdl2 sdl2_mixer sdl2_image sdl2_ttf glew physfs

## MacOS ##
.PHONY: install-dependencies-darwin
install-dependencies-darwin:
	xargs brew install < BrewDeps.txt
	xargs brew link < BrewDeps.txt


#### Docker related build rules ####

# Build rules relating to Docker images

DockerFolder := ${TopLevelFolder}/docker
DockerRunFlags := --volume ${TopLevelFolder}:/code
DockerRepository := outpostuniverse

ImageName := nas2d
ImageVersion := 1.4

ImageName_gcc := nas2d-gcc
ImageVersion_gcc := 1.3

ImageName_clang := nas2d-clang
ImageVersion_clang := 1.2

ImageName_mingw := nas2d-mingw
ImageVersion_mingw := 1.6

.PHONY: build-image
build-image:
	docker build ${DockerFolder}/ --file ${DockerFolder}/${ImageName}.Dockerfile --tag ${DockerRepository}/${ImageName}:latest --tag ${DockerRepository}/${ImageName}:${ImageVersion}

.PHONY: run-image
run-image:
	docker run ${DockerRunFlags} --rm --tty ${DockerRepository}/${ImageName}

.PHONY: debug-image
debug-image:
	docker run ${DockerRunFlags} --rm --tty --interactive ${DockerRepository}/${ImageName} bash

.PHONY: root-debug-image
root-debug-image:
	docker run ${DockerRunFlags} --rm --tty --interactive --user=0 ${DockerRepository}/${ImageName} bash

.PHONY: push-image
push-image:
	docker push ${DockerRepository}/${ImageName}

.PHONY: build-image-gcc
build-image-gcc: ImageName := ${ImageName_gcc}
build-image-gcc: ImageVersion := ${ImageVersion_gcc}
build-image-gcc: | build-image
.PHONY: run-image-gcc
run-image-gcc: ImageName := ${ImageName_gcc}
run-image-gcc: | run-image
.PHONY: debug-image-gcc
debug-image-gcc: ImageName := ${ImageName_gcc}
debug-image-gcc: | debug-image
.PHONY: root-debug-image-gcc
root-debug-image-gcc: ImageName := ${ImageName_gcc}
root-debug-image-gcc: | root-debug-image
.PHONY: push-image-gcc
push-image-gcc: ImageName := ${ImageName_gcc}
push-image-gcc: | push-image

.PHONY: build-image-clang
build-image-clang: ImageName := ${ImageName_clang}
build-image-clang: ImageVersion := ${ImageVersion_clang}
build-image-clang: | build-image
.PHONY: run-image-clang
run-image-clang: ImageName := ${ImageName_clang}
run-image-clang: | run-image
.PHONY: debug-image-clang
debug-image-clang: ImageName := ${ImageName_clang}
debug-image-clang: | debug-image
.PHONY: root-debug-image-clang
root-debug-image-clang: ImageName := ${ImageName_clang}
root-debug-image-clang: | root-debug-image
.PHONY: push-image-clang
push-image-clang: ImageName := ${ImageName_clang}
push-image-clang: | push-image

.PHONY: build-image-mingw
build-image-mingw: ImageName := ${ImageName_mingw}
build-image-mingw: ImageVersion := ${ImageVersion_mingw}
build-image-mingw: | build-image
.PHONY: run-image-mingw
run-image-mingw: ImageName := ${ImageName_mingw}
run-image-mingw: | run-image
.PHONY: debug-image-mingw
debug-image-mingw: ImageName := ${ImageName_mingw}
debug-image-mingw: | debug-image
.PHONY: root-debug-image-mingw
root-debug-image-mingw: ImageName := ${ImageName_mingw}
root-debug-image-mingw: | root-debug-image
.PHONY: push-image-mingw
push-image-mingw: ImageName := ${ImageName_mingw}
push-image-mingw: | push-image

#### CircleCI related build rules ####

.PHONY: circleci-validate circleci-build

circleci-validate:
	circleci config validate
circleci-build:
	circleci build
