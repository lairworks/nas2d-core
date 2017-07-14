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

