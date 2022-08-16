# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

# Capture top level folder before any Makefile includes
# Note: MAKEFILE_LIST's last entry is the last processed Makefile.
#       That should be the current Makefile, assuming no includes
TopLevelFolder := $(abspath $(dir $(lastword ${MAKEFILE_LIST})))

CONFIG = debug
debug_CXX_FLAGS := -Og -g
release_CXX_FLAGS := -O3
CONFIG_CXX_FLAGS := $($(CONFIG)_CXX_FLAGS)

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
CXXFLAGS_WARN := -Wall -Wextra -Wpedantic -Wzero-as-null-pointer-constant -Wnull-dereference -Wold-style-cast -Wcast-qual -Wcast-align -Wdouble-promotion -Wshadow -Wnon-virtual-dtor -Woverloaded-virtual -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wmissing-format-attribute -Wredundant-decls -Wformat=2 $(WARN_EXTRA)
CXXFLAGS := $(CXXFLAGS_EXTRA) $(CONFIG_CXX_FLAGS) -std=c++20 $(CXXFLAGS_WARN) $(SDL_CONFIG_CFLAGS)
LDFLAGS := $(LDFLAGS_EXTRA)
LDLIBS := $(LDLIBS_EXTRA) -lstdc++ -lSDL2_image -lSDL2_mixer -lSDL2_ttf $(SDL_CONFIG_LIBS) $(OpenGL_LIBS)

Windows_RUN_PREFIX := wine
RUN_PREFIX := $($(TARGET_OS)_RUN_PREFIX)

SRCDIR := NAS2D
ROOTBUILDDIR := .build
BUILDDIR := $(ROOTBUILDDIR)/$(CONFIG)
BINDIR := lib
INTDIR := $(BUILDDIR)/intermediate
OUTPUT := $(BINDIR)/libnas2d.a
PACKAGEDIR := $(BUILDDIR)/package

DEPFLAGS = -MT $@ -MMD -MP -MF $(INTDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(INTDIR)/$*.Td $(INTDIR)/$*.d && touch $@

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.o,$(SRCS))

.PHONY: nas2d
nas2d: $(OUTPUT)

.PHONY: all
all: nas2d test test-graphics

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
PLATFORM = $(TARGET_OS).x64
PACKAGE_NAME = $(PACKAGEDIR)/nas2d-$(VERSION)-$(PLATFORM)-$(CONFIG).tar.gz
Darwin_TAR_RENAME_FLAG := -s '!^$(SRCDIR)/!include/\0!'
Linux_TAR_RENAME_FLAG := --transform='s/^$(SRCDIR)/include\/\0/'
TAR_RENAME_FLAG := $($(CURRENT_OS)_TAR_RENAME_FLAG)

.PHONY: package
package: $(PACKAGE_NAME)

$(PACKAGE_NAME): $(OUTPUT) $(shell find $(SRCDIR) -name '*.h')
	@mkdir -p "${@D}"
	# Package an "include/" folder containing all header files, plus the library file
	find $(SRCDIR) -name '*.h' | tar -czf $(PACKAGE_NAME) $(TAR_RENAME_FLAG) -T - $(OUTPUT)


.PHONY: clean clean-all
clean:
	-rm --force --recursive $(INTDIR)
clean-all: | clean
	-rm --force --recursive $(ROOTBUILDDIR)
	-rm --force $(OUTPUT)
	-rm --force --dir $(BINDIR)


## Unit Test project ##

TESTDIR := test
TESTINTDIR := $(BUILDDIR)/testIntermediate
TESTSRCS := $(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTINTDIR)/%.o,$(TESTSRCS))
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
$(BUILDDIR)/testGraphics: test-graphics/*.cpp test-graphics/*.h $(OUTPUT)
	@mkdir -p "${@D}"
	$(CXX) -o $@ test-graphics/*.cpp $(TESTCPPFLAGS) $(CXXFLAGS) -Umain $(TESTLDFLAGS) -lnas2d $(LDLIBS)

.PHONY: run-test-graphics
run-test-graphics: | test-graphics
	cd test-graphics/ && ../$(BUILDDIR)/testGraphics ; cd ..


.PHONY: lint
lint: cppcheck cppclean

.PHONY: cppcheck
cppcheck:
	cppcheck --quiet "$(SRCDIR)"

.PHONY: cppclean
cppclean:
	cppclean "$(SRCDIR)" --exclude="NAS2D.h" --exclude="Xml"

.PHONY: format
format:
	clang-format --version
	find NAS2D/ -path NAS2D/Xml -prune -name '*.cpp' -o -name '*.h' | xargs clang-format -i

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
	apt --yes install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev

## CentOS ##
.PHONY: install-dependencies-centos
install-dependencies-centos: | install-dependencies-repository-centos
	# Install development packages (-y answers "yes" to prompts)
	yum --assumeyes install SDL2-devel SDL2_mixer-devel SDL2_image-devel SDL2_ttf-devel glew-devel
.PHONY: install-dependencies-repository-centos
install-dependencies-repository-centos:
	# Default CentOS repositories only contain SDL1
	# For SDL2 use EPEL repo (EPEL = Extra Packages for Enterprise Linux)
	yum --assumeyes install epel-release

## Arch Linux ##
.PHONY: install-dependencies-arch
install-dependencies-arch:
	pacman --sync --refresh sdl2 sdl2_mixer sdl2_image sdl2_ttf glew

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

ImageVersion_gcc := 1.4
ImageVersion_clang := 1.3
ImageVersion_mingw := 1.8
ImageVersion_arch := 1.1

DockerImageName = ${DockerRepository}/nas2d-$*:${ImageVersion_$*}

DockerBuildRules := build-image-gcc build-image-clang build-image-mingw build-image-arch
DockerRunRules := run-image-gcc run-image-clang run-image-mingw run-image-arch
DockerDebugRules := debug-image-gcc debug-image-clang debug-image-mingw debug-image-arch
DockerDebugRootRules := root-debug-image-gcc root-debug-image-clang root-debug-image-mingw root-debug-image-arch
DockerPushRules := push-image-gcc push-image-clang push-image-mingw push-image-arch

.PHONY: ${DockerBuildRules} ${DockerRunRules} ${DockerDebugRules} ${DockerDebugRootRules} ${DockerPushRules}

${DockerBuildRules}: build-image-%:
	docker build ${DockerFolder}/ --file ${DockerFolder}/nas2d-$*.Dockerfile --tag ${DockerImageName} --tag ${DockerRepository}/nas2d-$*:latest

${DockerRunRules}: run-image-%:
	docker run ${DockerRunFlags} --rm --tty ${DockerImageName}

${DockerDebugRules}: debug-image-%:
	docker run ${DockerRunFlags} --rm --tty --interactive ${DockerImageName} bash

${DockerDebugRootRules}: root-debug-image-%:
	docker run ${DockerRunFlags} --rm --tty --interactive --user=0 ${DockerImageName} bash

${DockerPushRules}: push-image-%:
	docker push ${DockerImageName}
	docker push ${DockerRepository}/nas2d-$*:latest

#### CircleCI related build rules ####

.PHONY: circleci-validate circleci-build

circleci-validate:
	circleci config validate
circleci-build:
	circleci build
