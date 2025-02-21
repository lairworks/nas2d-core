# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

CONFIG = Debug
Debug_CXX_FLAGS := -Og -g
Release_CXX_FLAGS := -O3
CONFIG_CXX_FLAGS := $($(CONFIG)_CXX_FLAGS)

# Determine OS (Linux, Darwin, ...)
CURRENT_OS := $(shell uname 2>/dev/null || echo Unknown)
TARGET_OS ?= $(CURRENT_OS)

Windows_RUN_PREFIX := wine
RUN_PREFIX := $($(TARGET_OS)_RUN_PREFIX)

Windows_EXE_SUFFIX := .exe
EXE_SUFFIX := $($(TARGET_OS)_EXE_SUFFIX)

ROOTBUILDDIR := .build
BUILDDIRPREFIX := $(ROOTBUILDDIR)/$(CONFIG)_Linux_


## Default and top-level targets ##

.DEFAULT_GOAL := nas2d

.PHONY: all
all: nas2d test test-graphics


## NAS2D project ##

SRCDIR := NAS2D
BINDIR := lib
INTDIR := $(BUILDDIRPREFIX)nas2d/intermediate
OUTPUT := $(BINDIR)/libnas2d.a
SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.o,$(SRCS))

Linux_OpenGL_LIBS := -lGLEW -lGL
Darwin_OpenGL_LIBS := -lGLEW -framework OpenGL
Windows_OpenGL_LIBS := -lglew32 -lopengl32
OpenGL_LIBS := $($(TARGET_OS)_OpenGL_LIBS)

SDL_LIBS := -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2

CPPFLAGS := $(CPPFLAGS_EXTRA)
CXXFLAGS_WARN := -Wall -Wextra -Wpedantic -Wzero-as-null-pointer-constant -Wnull-dereference -Wold-style-cast -Wcast-qual -Wcast-align -Wdouble-promotion -Wshadow -Wnon-virtual-dtor -Woverloaded-virtual -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wmissing-format-attribute -Wredundant-decls -Wformat=2 $(WARN_EXTRA)
CXXFLAGS := $(CXXFLAGS_EXTRA) $(CONFIG_CXX_FLAGS) -std=c++20 $(CXXFLAGS_WARN)
LDFLAGS := $(LDFLAGS_EXTRA)
LDLIBS := $(LDLIBS_EXTRA) -lstdc++ $(SDL_LIBS) $(OpenGL_LIBS)

PROJECT_FLAGS = $(CPPFLAGS) $(CXXFLAGS)

.PHONY: nas2d
nas2d: $(OUTPUT)

$(OUTPUT): $(OBJS)
$(OBJS): $(INTDIR)/%.o : $(SRCDIR)/%.cpp $(INTDIR)/%.dep

-include $(patsubst %.o,%.dep,$(OBJS))


## Unit Test project ##

TESTDIR := test
TESTINTDIR := $(BUILDDIRPREFIX)test/intermediate
TESTOUTPUT := $(BUILDDIRPREFIX)test/test$(EXE_SUFFIX)
TESTSRCS := $(shell find $(TESTDIR) -name '*.cpp')
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTINTDIR)/%.o,$(TESTSRCS))

TESTCPPFLAGS := $(CPPFLAGS) -I./
TESTLDFLAGS := $(LDFLAGS)
TESTLIBS := -lgmock_main -lgmock -lgtest -lpthread $(LDLIBS)

TESTPROJECT_FLAGS = $(TESTCPPFLAGS) $(CXXFLAGS)
TESTPROJECT_LINKFLAGS = $(TESTLDFLAGS) $(TESTLIBS)

.PHONY: test
test: $(TESTOUTPUT)

$(TESTOUTPUT): PROJECT_LINKFLAGS = $(TESTPROJECT_LINKFLAGS)
$(TESTOUTPUT): $(TESTOBJS) $(OUTPUT)

$(TESTOBJS): PROJECT_FLAGS = $(TESTPROJECT_FLAGS)
$(TESTOBJS): $(TESTINTDIR)/%.o : $(TESTDIR)/%.cpp $(TESTINTDIR)/%.dep

-include $(patsubst %.o,%.dep,$(TESTOBJS))


.PHONY: check
check: | test
	cd test && $(RUN_PREFIX) ../$(TESTOUTPUT) $(GTEST_OPTIONS)


## Graphics test project ##

TESTGRAPHICSDIR := test-graphics
TESTGRAPHICSINTDIR := $(BUILDDIRPREFIX)testGraphics/intermediate
TESTGRAPHICSOUTPUT := $(BUILDDIRPREFIX)testGraphics/testGraphics$(EXE_SUFFIX)
TESTGRAPHICSSRCS := $(shell find $(TESTGRAPHICSDIR) -name '*.cpp')
TESTGRAPHICSOBJS := $(patsubst $(TESTGRAPHICSDIR)/%.cpp,$(TESTGRAPHICSINTDIR)/%.o,$(TESTGRAPHICSSRCS))

TESTGRAPHICSPROJECT_FLAGS = $(TESTCPPFLAGS) $(CXXFLAGS)
TESTGRAPHICSPROJECT_LINKFLAGS = $(TESTLDFLAGS) $(LDLIBS)

.PHONY: test-graphics
test-graphics: $(TESTGRAPHICSOUTPUT)

$(TESTGRAPHICSOUTPUT): PROJECT_LINKFLAGS = $(TESTGRAPHICSPROJECT_LINKFLAGS)
$(TESTGRAPHICSOUTPUT): $(TESTGRAPHICSOBJS) $(OUTPUT)

$(TESTGRAPHICSOBJS): PROJECT_FLAGS = $(TESTGRAPHICSPROJECT_FLAGS)
$(TESTGRAPHICSOBJS): $(TESTGRAPHICSINTDIR)/%.o : $(TESTGRAPHICSDIR)/%.cpp $(TESTGRAPHICSINTDIR)/%.dep

-include $(patsubst %.o,%.dep,$(TESTGRAPHICSOBJS))


.PHONY: run-test-graphics
run-test-graphics: | test-graphics
	cd test-graphics/ && ../$(TESTGRAPHICSOUTPUT) ; cd ..


## Compile rules ##

DEPFLAGS = -MMD -MP
COMPILE.cpp = $(CXX) $(DEPFLAGS) $(PROJECT_FLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(@:.o=.d) $(@:.o=.dep) && touch $@


%:
	@mkdir -p "${@D}"
	$(CXX) $^ $(PROJECT_LINKFLAGS) -o $@

lib%.a:
	@mkdir -p "${@D}"
	ar rcs $@ $^

%.o:
	@mkdir -p "${@D}"
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.dep: ;
.PRECIOUS: %.dep


## Clean ##

.PHONY: clean clean-all
clean:
	-rm --force --recursive $(INTDIR)
clean-all: | clean
	-rm --force --recursive $(ROOTBUILDDIR)
	-rm --force $(OUTPUT)
	-rm --force --dir $(BINDIR)


## Package ##

PACKAGEDIR := $(ROOTBUILDDIR)/package
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


## Linting ##

.PHONY: show-warnings
show-warnings:
	@$(MAKE) clean > /dev/null
	$(MAKE) --output-sync all CXX=clang++ CXXFLAGS_WARN=-Weverything 2>&1 >/dev/null | grep -o "\[-W.*\]" | sort | uniq
	@$(MAKE) clean > /dev/null

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
	find NAS2D/ -not -path 'NAS2D/Xml/*' -regex '.*\.\(cpp\|h\)' | xargs clang-format -i

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

include dockerBuildEnv/makefile

#### CircleCI related build rules ####

.PHONY: circleci-validate circleci-build

circleci-validate:
	circleci config validate
circleci-build:
	circleci build
