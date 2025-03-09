# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

## Default and top-level targets ##

.DEFAULT_GOAL := nas2d

.PHONY: all
all: nas2d test demoGraphics

# Determine OS (Linux, Darwin, ...)
CURRENT_OS := $(shell uname 2>/dev/null || echo Unknown)
TARGET_OS ?= $(CURRENT_OS)

# Toolchain: gcc, clang, mingw, (or blank for environment default)
Toolchain :=

PkgConfig := pkg-config

gccCXX := g++
gccPkgConfig := $(PkgConfig)
gccTARGET_OS := $(TARGET_OS)
clangCXX := clang++
clangPkgConfig := $(PkgConfig)
clangTARGET_OS := $(TARGET_OS)
mingwCXX := x86_64-w64-mingw32-g++
mingwPkgConfig := x86_64-w64-mingw32-pkg-config
mingwTARGET_OS := Windows

CXX := $($(Toolchain)CXX)
PkgConfig := $($(Toolchain)PkgConfig)
TARGET_OS := $($(Toolchain)TARGET_OS)

# Build configuration
CONFIG = Debug

Debug_CXX_FLAGS := -Og -g
Release_CXX_FLAGS := -O3
CONFIG_CXX_FLAGS := $($(CONFIG)_CXX_FLAGS)

# Target specific settings
WindowsSpecialPreprocessorFlags = -DGLEW_STATIC
WindowsSpecialWarnFlags = -Wno-redundant-decls
WindowsExeSuffix := .exe
WindowsRunPrefix := wine

SpecialPreprocessorFlags := $($(TARGET_OS)SpecialPreprocessorFlags)
SpecialWarnFlags := $($(TARGET_OS)SpecialWarnFlags)
ExeSuffix := $($(TARGET_OS)ExeSuffix)
RunPrefix := $($(TARGET_OS)RunPrefix)

ROOTBUILDDIR := .build
BUILDDIRPREFIX := $(ROOTBUILDDIR)/$(CONFIG)_Linux_


## NAS2D project ##

SRCDIR := NAS2D
BINDIR := lib
INTDIR := $(BUILDDIRPREFIX)nas2d/intermediate
OUTPUT := $(BINDIR)/libnas2d.a
SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.o,$(SRCS))

IncludeSearchPath := $(shell type $(PkgConfig) >/dev/null 2>&1 && $(PkgConfig) --cflags-only-I sdl2)
LibrarySearchPath := $(shell type $(PkgConfig) >/dev/null 2>&1 && $(PkgConfig) --libs-only-L sdl2)

Linux_OpenGL_LIBS := -lGLEW -lGL
Darwin_OpenGL_LIBS := -lGLEW -framework OpenGL
Windows_OpenGL_LIBS := -lglew32 -lopengl32
OpenGL_LIBS := $($(TARGET_OS)_OpenGL_LIBS)

SDL_LIBS := -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2

CPPFLAGS := $(IncludeSearchPath) $(SpecialPreprocessorFlags) $(CPPFLAGS_EXTRA)
CXXFLAGS_WARN := -Wall -Wextra -Wpedantic -Wzero-as-null-pointer-constant -Wnull-dereference -Wold-style-cast -Wcast-qual -Wcast-align -Wdouble-promotion -Wshadow -Wnon-virtual-dtor -Woverloaded-virtual -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wmissing-format-attribute -Wredundant-decls -Wformat=2 $(SpecialWarnFlags) $(WARN_EXTRA)
CXXFLAGS := $(CXXFLAGS_EXTRA) $(CONFIG_CXX_FLAGS) -std=c++20 $(CXXFLAGS_WARN)
LDFLAGS := $(LibrarySearchPath) $(LDFLAGS_EXTRA)
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
TESTOUTPUT := $(BUILDDIRPREFIX)test/test$(ExeSuffix)
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
	cd test && $(RunPrefix) ../$(TESTOUTPUT) $(GTEST_OPTIONS)


## Graphics demo project ##

DEMOGRAPHICSDIR := demoGraphics
DEMOGRAPHICSINTDIR := $(BUILDDIRPREFIX)demoGraphics/intermediate
DEMOGRAPHICSOUTPUT := $(BUILDDIRPREFIX)demoGraphics/demoGraphics$(ExeSuffix)
DEMOGRAPHICSSRCS := $(shell find $(DEMOGRAPHICSDIR) -name '*.cpp')
DEMOGRAPHICSOBJS := $(patsubst $(DEMOGRAPHICSDIR)/%.cpp,$(DEMOGRAPHICSINTDIR)/%.o,$(DEMOGRAPHICSSRCS))

DEMOGRAPHICSPROJECT_FLAGS = $(TESTCPPFLAGS) $(CXXFLAGS)
DEMOGRAPHICSPROJECT_LINKFLAGS = $(TESTLDFLAGS) $(LDLIBS)

.PHONY: demoGraphics
demoGraphics: $(DEMOGRAPHICSOUTPUT)

$(DEMOGRAPHICSOUTPUT): PROJECT_LINKFLAGS = $(DEMOGRAPHICSPROJECT_LINKFLAGS)
$(DEMOGRAPHICSOUTPUT): $(DEMOGRAPHICSOBJS) $(OUTPUT)

$(DEMOGRAPHICSOBJS): PROJECT_FLAGS = $(DEMOGRAPHICSPROJECT_FLAGS)
$(DEMOGRAPHICSOBJS): $(DEMOGRAPHICSINTDIR)/%.o : $(DEMOGRAPHICSDIR)/%.cpp $(DEMOGRAPHICSINTDIR)/%.dep

-include $(patsubst %.o,%.dep,$(DEMOGRAPHICSOBJS))


.PHONY: run-demoGraphics
run-demoGraphics: | demoGraphics
	cd demoGraphics/ && $(RunPrefix) ../$(DEMOGRAPHICSOUTPUT) ; cd ..


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

WarnNoUninteresting := -Wno-c++98-compat-pedantic -Wno-pre-c++17-compat

.PHONY: show-warnings
show-warnings:
	@$(MAKE) clean-all > /dev/null
	$(MAKE) --output-sync all CXX=clang++ CXXFLAGS_WARN="-Weverything $(WarnNoUninteresting)" 2>&1 >/dev/null | grep -o "\[-W.*\]" | sort | uniq
	@$(MAKE) clean-all > /dev/null

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


## GitHub ##
.PHONY: cache-list-all cache-list-main cache-list-branch cache-delete-main-stale cache-delete-branch
GhCacheKeyIncremental := buildCache-
GhCacheLimit := 100
GhCacheFields := id,ref,key,version,sizeInBytes,createdAt,lastAccessedAt
GhCacheListJson := gh cache list --limit $(GhCacheLimit) --json $(GhCacheFields)
GhCacheListMain := $(GhCacheListJson) --ref refs/heads/main --key $(GhCacheKeyIncremental) --jq 'sort_by(.lastAccessedAt)'
GhCacheListBranch := $(GhCacheListJson) --jq '.[] | select(.ref!="refs/heads/main")'
GhCacheDeleteIds := xargs -I '{}' gh cache delete '{}'

cache-list-all:
	$(GhCacheListJson)

cache-list-main:
	$(GhCacheListMain)

cache-list-branch:
	$(GhCacheListBranch)

cache-delete-main-stale:
	$(GhCacheListMain) | jq '.[0:-2] | .[] .id' | $(GhCacheDeleteIds)

cache-delete-branch:
	$(GhCacheListBranch) | jq '.id' | $(GhCacheDeleteIds)
