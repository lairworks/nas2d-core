# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:22.04

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
# Set DEBIAN_FRONTEND to prevent tzdata package install from prompting for timezone
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential=12.9* \
    clang=1:14.0-* \
    cmake=3.22.1-* \
    libgtest-dev=1.11.0-* \
    libgmock-dev=1.11.0-* \
    git=1:2.34.1-* \
    ssh=1:8.9p1-* \
    tar=1.34* \
    gzip=1.10-* \
    ca-certificates=* \
  && rm -rf /var/lib/apt/lists/*

ENV CXX=clang++
ENV  CC=clang

# Install NAS2D specific dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libglew-dev=2.2.0-* \
    libsdl2-dev=2.0.20+* \
    libsdl2-image-dev=2.0.5+* \
    libsdl2-mixer-dev=2.0.4+* \
    libsdl2-ttf-dev=2.0.18+* \
  && rm -rf /var/lib/apt/lists/*

CMD ["make", "--keep-going", "check"]
