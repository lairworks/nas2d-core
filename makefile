# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

# Capture top level folder before any Makefile includes
# Note: MAKEFILE_LIST's last entry is the last processed Makefile.
#       That should be the current Makefile, assuming no includes
TopLevelFolder := $(abspath $(dir $(lastword ${MAKEFILE_LIST})))

SRCDIR := src
INCDIR := include
BUILDDIR := .build
BINDIR := lib
INTDIR := $(BUILDDIR)/intermediate
OUTPUT := $(BINDIR)/libnas2d.a

# Determine OS (Linux, Darwin, ...)
CURRENT_OS := $(shell uname 2>/dev/null || echo Unknown)
TARGET_OS ?= $(CURRENT_OS)

Linux_OpenGL_LIBS := -lGLEW -lGL
Darwin_OpenGL_LIBS := -lGLEW -framework OpenGL
Windows_OpenGL_LIBS := -lglew32 -lopengl32
OpenGL_LIBS := $($(TARGET_OS)_OpenGL_LIBS)

CPPFLAGS := $(CPPFLAGS.EXTRA) -Iinclude/
CXXFLAGS := $(CXXFLAGS.EXTRA) -std=c++17 -Wall -Wpedantic -Werror $(shell sdl2-config --cflags)
LDFLAGS := $(LDFLAGS.EXTRA)
LDLIBS := $(LDLIBS.EXTRA) -lstdc++ -lphysfs -lSDL2_image -lSDL2_mixer -lSDL2_ttf $(shell sdl2-config --static-libs) $(OpenGL_LIBS)

Windows_RUN_PREFIX := wine
RUN_PREFIX := $($(TARGET_OS)_RUN_PREFIX)

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
TESTCPPFLAGS := $(CPPFLAGS)
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


.PHONY: cppcheck
cppcheck:
	cppcheck --quiet "$(SRCDIR)"

.PHONY: cppclean
cppclean:
	cppclean --include-path "$(INCDIR)" "$(SRCDIR)"

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

DockerFolder := ${TopLevelFolder}/docker
DockerRunFlags := --volume ${TopLevelFolder}:/code
DockerRepository := outpostuniverse

ImageName := nas2d
ImageVersion := 1.4

ImageName_gcc8 := nas2d-gcc8
ImageVersion_gcc8 := 1.0

ImageName_clang := nas2d-clang
ImageVersion_clang := 1.0

ImageName_mingw := nas2d-mingw
ImageVersion_mingw := 1.3

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

.PHONY: build-image-gcc8
build-image-gcc8: ImageName := ${ImageName_gcc8}
build-image-gcc8: ImageVersion := ${ImageVersion_gcc8}
build-image-gcc8: | build-image
.PHONY: run-image-gcc8
run-image-gcc8: ImageName := ${ImageName_gcc8}
run-image-gcc8: | run-image
.PHONY: debug-image-gcc8
debug-image-gcc8: ImageName := ${ImageName_gcc8}
debug-image-gcc8: | debug-image
.PHONY: root-debug-image-gcc8
root-debug-image-gcc8: ImageName := ${ImageName_gcc8}
root-debug-image-gcc8: | root-debug-image
.PHONY: push-image-gcc8
push-image-gcc8: ImageName := ${ImageName_gcc8}
push-image-gcc8: | push-image

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
