# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:24.04

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
# Install latest available GCC compiler (which may not be the default)
# Set DEBIAN_FRONTEND to prevent tzdata package install from prompting for timezone
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    g++=4:13.2.0-* \
    make=4.3-* \
    cmake=3.28.3-* \
    libgtest-dev=1.14.0-* \
    libgmock-dev=1.14.0-* \
    git=1:2.43.0-* \
    ssh=1:9.6p1-* \
    tar=1.35+* \
    gzip=1.12-* \
    ca-certificates=* \
  && rm -rf /var/lib/apt/lists/*

ENV CXX=g++
ENV  CC=gcc

# Install NAS2D specific dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libglew-dev=2.2.0-* \
    libsdl2-dev=2.30.0+* \
    libsdl2-image-dev=2.8.2+* \
    libsdl2-mixer-dev=2.8.0+* \
    libsdl2-ttf-dev=2.22.0+* \
  && rm -rf /var/lib/apt/lists/*

CMD ["make", "--keep-going", "check"]
