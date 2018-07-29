# Build from the root project folder with:
#   docker build docker/ --file docker/Ubuntu-18.04.BuildEnv.Dockerfile --tag ubuntu-18.04-gcc-sdl2
# Run the resulting image with:
#   docker run --rm --tty --volume `pwd`:/code ubuntu-18.04-gcc-sdl2

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
