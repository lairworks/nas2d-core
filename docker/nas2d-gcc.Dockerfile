# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:20.04

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
# Install latest available GCC compiler (which may not be the default)
# Set DEBIAN_FRONTEND to prevent tzdata package install from prompting for timezone
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    g++-10=10.3.0-* \
    make=4.2.1-* \
    cmake=3.16.3-* \
    libgtest-dev=1.10.0-* \
    libgmock-dev=1.10.0-* \
    git=1:2.25.1-* \
    ssh=1:8.2p1-* \
    tar=1.30+* \
    gzip=1.10-* \
    ca-certificates=* \
  && rm -rf /var/lib/apt/lists/*

ENV CXX=g++-10
ENV  CC=gcc-10

# Install NAS2D specific dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libglew-dev=2.1.0-* \
    libphysfs-dev=3.0.2-* \
    libsdl2-dev=2.0.10+* \
    libsdl2-image-dev=2.0.5+* \
    libsdl2-mixer-dev=2.0.4+* \
    libsdl2-ttf-dev=2.0.15+* \
  && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash user
USER user

VOLUME /code
WORKDIR /code

CMD ["make", "--keep-going", "check"]
