# Build from the root project folder with:
#   docker build docker/ --file docker/Ubuntu-18.04.BuildEnv.Dockerfile --tag ubuntu-18.04-gcc-sdl2
# Run the resulting image to compile source with:
#   docker run --rm --tty --volume `pwd`:/code ubuntu-18.04-gcc-sdl2

# Debug build environment interactively as normal user with:
#   docker run --rm --tty --volume `pwd`:/code --interactive ubuntu-18.04-gcc-sdl2 bash
# Debug build environment interactively as root with:
#   docker run --rm --tty --volume `pwd`:/code --interactive --user=0 ubuntu-18.04-gcc-sdl2 bash

FROM ubuntu:18.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    libglew-dev \
    libphysfs-dev \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev \
  && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash user
USER user

VOLUME /code
WORKDIR /code

CMD ["make", "-k"]
