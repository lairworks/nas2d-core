# Source http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

SRC = ./src
INC = ./include
BIN = ./lib

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

CFLAGS = -std=c++11 -g -Wall -I$(INC) `sdl2-config --cflags`
LDFLAGS = -lstdc++ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLU -lGL

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

SRCS = $(shell find $(SRC) -name '*.cpp')
OBJ = $(patsubst %.cpp,%.o,$(SRCS))
EXE = $(BIN)/libnas2d.a

all: $(EXE)

$(EXE): $(OBJ)
	@mkdir -p ${@D}
	ar rcs $@ $^

%.o : %.cpp
$(OBJ): %.o : %.cpp $(DEPDIR)/%.d
	@mkdir -p $(?D) > /dev/null
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))

.PHONY:clean
clean:
	-rm -fr $(OBJ)
	-rm -fr $(EXE)

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
	pacman -S sdl2 sdl2_mixer sdl2_image sdl2_ttf glew glee physfs


## Ubuntu ##

.PHONY:install-deps-ubuntu
install-deps-ubuntu:
	apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev glee-dev libphysfs-dev


## CentOS ##

.PHONY:install-repos-centos
install-repos-centos:
	# Default CentOS repositories only contain SDL1
	# For SDL2 use EPEL repo (EPEL = Extra Packages for Enterprise Linux)
	yum install epel-release
	# For GLee use Nux Dextop repo
	# **Note**: Nux Dextop might conflict with other extension repositories
	# GLee manages OpenGL extensions, much like Glew
	# GLee appears to be old an unmaintained.
	# (-y answers "yes" to prompts)
	yum install -y http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-1.el7.nux.noarch.rpm

.PHONY:install-deps-centos
install-deps-centos:
	# Install development packages (-y answers "yes" to prompts)
	yum -y install SDL2-devel SDL2_mixer-devel SDL2_image-devel SDL2_ttf-devel glew-devel physfs-devel GLee-devel

