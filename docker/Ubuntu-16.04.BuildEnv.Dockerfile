# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:16.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++=4:5.3.1-* \
    make=4.1-6 \
    cmake=3.5.1-* \
    wget=1.17.1-* \
    bzip2=1.0.6-8 \
    ca-certificates=* \
    libfreetype6-dev=2.6.1-* \
    libglew-dev=1.13.0-2 \
    libphysfs-dev=2.0.3-3 \
  && rm -rf /var/lib/apt/lists/*

# Download and install dependencies from source
COPY Makefile /buildDependencies/build/
RUN cd /buildDependencies/build/ && make compile-sdl2 && make install-sdl2 && make clean-all-sdl2
RUN cd /buildDependencies/build/ && make compile-sdl2-modules && make install-sdl2-modules && make clean-all-sdl2-modules
RUN cd /buildDependencies/build/ && make compile-physfs && make install-physfs && make clean-physfs

RUN useradd user
USER user

VOLUME /code
WORKDIR /code

# The MAINTAINER tag was deprecated in Docker 1.13. The alternative is to use a LABEL tag.
# This is probably better off not being in the Dockerfile at all though for open source projects.
# Specifying maintainers in source files is known to discourage other people from making updates.
MAINTAINER "WhoEverWantsToEdit <anybody@anywhere.com>"

CMD ["make", "--keep-going", "check"]
