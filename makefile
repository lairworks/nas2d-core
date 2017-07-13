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



# Arch Linux

.PHONY:install-deps-arch
install-deps-arch:
	pacman -S sdl2 sdl2_mixer sdl2_image sdl2_ttf glew glee physfs

# Ubuntu

.PHONY:install-deps-ubuntu
install-deps-ubuntu:
	apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev glee-dev libphysfs-dev
