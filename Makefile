DIR_INC		=	./include
DIR_SRC		=	./src
DIR_OBJ		=	./obj
DIR_BIN		=	./lib

CFLAGS		+=	-std=c++11 -g -Wall -I${DIR_INC} `sdl2-config --cflags`
LDFLAGS		+=	-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLU -lGL

SRC			=	$(shell find ${DIR_SRC} -name "*.cpp")
OBJ			=	$(subst src/,obj/,$(SRC:.cpp=.o))
INC_H		:=	$(shell find $(DIR_INC) -name '*.h')

TARGET		=	${DIR_BIN}/libnas2d.a

all: ${TARGET}

${TARGET}: ${OBJ}
	@mkdir -p ${@D}
	ar rcs $@ $^

${DIR_OBJ}/%.o: ${DIR_SRC}/%.cpp ${INC_H}
	@mkdir -p ${@D}
	${CXX} -c $< ${CFLAGS} -o $@

.PHONY:clean
clean:
	-rm -fr ${DIR_OBJ}/*
	-rm -fr ${DIR_BIN}/*



# Arch Linux

.PHONY:install-deps-arch
install-deps-arch:
	pacman -S sdl2 sdl2_mixer sdl2_image sdl2_ttf glew glee physfs

# Ubuntu

.PHONY:install-deps-ubuntu
install-deps-ubuntu:
	apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev glee-dev libphysfs-dev
