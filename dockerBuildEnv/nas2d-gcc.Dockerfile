# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:resolute-20260610

# Remove automatic apt package cleanup so a local cache mount can be used
RUN rm /etc/apt/apt.conf.d/docker-clean

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
# Install latest available GCC compiler (which may not be the default)
# Set DEBIAN_FRONTEND to prevent tzdata package install from prompting for timezone
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    g++=4:15.2.0-* \
    make=4.4.1-* \
    cmake=4.2.3-* \
    libgtest-dev=1.17.0-* \
    libgmock-dev=1.17.0-* \
    git=1:2.53.0-* \
    ssh=1:10.2p1-* \
    tar=1.35+* \
    gzip=1.14-* \
    ca-certificates=*

ENV CXX=g++
ENV  CC=gcc

# Install NAS2D specific dependencies
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    libglew-dev=2.2.0-* \
    libsdl2-dev=2.32.10+* \
    libsdl2-image-dev=2.8.8+* \
    libsdl2-mixer-dev=2.8.1+* \
    libsdl2-ttf-dev=2.24.0+*

# Set custom variables for build script convenience
# Activate appropriate Toolchain settings
ENV Toolchain=gcc

CMD ["make", "--keep-going", "check"]
