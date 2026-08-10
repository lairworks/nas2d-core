# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:resolute-20260724.1

# Remove automatic apt package cleanup so a local cache mount can be used
RUN rm /etc/apt/apt.conf.d/docker-clean

# Install base development tools
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    clang=1:21.1.6-* \
    make=4.4.1-* \
    git=1:2.53.0-* \
    ca-certificates=* \
    libgtest-dev=1.17.0-* \
    libgmock-dev=1.17.0-*

ENV CXX=clang++
ENV  CC=clang

# Install NAS2D specific dependencies
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    libglew-dev=2.2.0-* \
    libsdl2-dev=2.32.10+* \
    libsdl2-image-dev=2.8.8+* \
    libsdl2-mixer-dev=2.8.1+* \
    libsdl2-ttf-dev=2.24.0+*

# Set custom variables for build script convenience
# Activate appropriate Toolchain settings
ENV Toolchain=clang

CMD ["make", "--keep-going", "check"]
