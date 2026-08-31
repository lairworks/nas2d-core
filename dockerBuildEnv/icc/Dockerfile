# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:resolute-20260724.1 AS base-image

# Remove automatic apt package cleanup so a local cache mount can be used
RUN rm /etc/apt/apt.conf.d/docker-clean


# ----

FROM base-image AS intel-apt-repo-key

RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends --quiet \
    gnupg=2.4.8-*

ADD --link https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB /tmp/intel-oneapi.asc

RUN gpg --output /tmp/apt.intel-oneapi.gpg --dearmor /tmp/intel-oneapi.asc


# ----

FROM base-image AS build-tools

# Install base development tools
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends --quiet \
    make=4.4.1-* \
    git=1:2.53.0-* \
    ca-certificates=*

# Install apt repository for Intel C++ Compiler
RUN \
  --mount=type=bind,from=intel-apt-repo-key,source=/tmp,target=/intel-key \
  install -D --mode=644 /intel-key/apt.intel-oneapi.gpg /etc/apt/keyrings/ && \
  echo "deb [signed-by=/etc/apt/keyrings/apt.intel-oneapi.gpg] https://apt.repos.intel.com/oneapi all main" > /etc/apt/sources.list.d/intel-oneapi.list

# Install Intel C++ Compiler (which needs ca-certificates to access apt repo)
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends --quiet \
    intel-oneapi-compiler-dpcpp-cpp=*

ENV \
  PATH=/opt/intel/oneapi/compiler/latest/bin/:${PATH} \
  CXX=icpx \
  CC=icx


# ----

FROM build-tools

RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends --quiet \
    libgtest-dev=1.17.0-* \
    libgmock-dev=1.17.0-*

# Install NAS2D specific dependencies
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends --quiet \
    libglew-dev=2.2.0-* \
    libsdl2-dev=2.32.10+* \
    libsdl2-image-dev=2.8.8+* \
    libsdl2-mixer-dev=2.8.1+* \
    libsdl2-ttf-dev=2.24.0+*

# Set custom variables for build script convenience
# Activate appropriate Toolchain settings
ENV Toolchain=

CMD ["make", "--keep-going", "check"]
