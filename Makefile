DIR_INC		=	./include
DIR_SRC		=	./src
DIR_OBJ		=	./obj
DIR_BIN		=	./lib

CFLAGS		=	-g -Wall -I${DIR_INC} `sdl-config --cflags`
LDFLAGS		=	-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLU -lGL

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
