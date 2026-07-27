# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:resolute-20260610 AS build-tools

# Remove automatic apt package cleanup so a local cache mount can be used
RUN rm /etc/apt/apt.conf.d/docker-clean

# Install base development tools
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    g++-mingw-w64-x86-64-win32=13.2.0-* \
    mingw-w64-tools \
    make=4.4.1-*

ENV TARGET_TRIPLET=x86_64-w64-mingw32
ENV \
  CXX=${TARGET_TRIPLET}-g++ \
  CC=${TARGET_TRIPLET}-gcc \
  LD=${TARGET_TRIPLET}-ld \
  AR=${TARGET_TRIPLET}-ar \
  STRIP=${TARGET_TRIPLET}-strip

# Custom variables describing the cross compile environment
ENV \
  GCC_RUNTIME_PATH=/usr/lib/gcc/${TARGET_TRIPLET}/13-win32/ \
  LOCAL_PACKAGE_PATH=/usr/local/${TARGET_TRIPLET}/


# ----

FROM build-tools AS dependencies

RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    curl=8.18.0-* \
    gnupg=2.4.8-* \
    tar=1.35+* \
    gzip=1.14-* \
    ca-certificates=*

# Install NAS2D specific dependencies
WORKDIR /tmp/
# Install SDL libraries from binary packages
RUN sdlVersion="2.32.10" && \
  curl --fail https://libsdl.org/release/SDL2-devel-${sdlVersion}-mingw.tar.gz | \
  tar --extract --gunzip && \
  make --directory SDL2-${sdlVersion}/ cross && \
  rm --force --recursive SDL2-${sdlVersion}/
RUN sdlImageVersion="2.8.12" && \
  curl --fail https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-${sdlImageVersion}-mingw.tar.gz | \
  tar --extract --gunzip && \
  make --directory SDL2_image-${sdlImageVersion}/ cross && \
  rm --force --recursive SDL2_image-${sdlImageVersion}/
RUN sdlMixerVersion="2.8.2" && \
  curl --fail https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-${sdlMixerVersion}-mingw.tar.gz | \
  tar --extract --gunzip && \
  make --directory SDL2_mixer-${sdlMixerVersion}/ cross && \
  rm --force --recursive SDL2_mixer-${sdlMixerVersion}/
RUN sdlTtfVersion="2.24.0" && \
  curl --fail https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-${sdlTtfVersion}-mingw.tar.gz | \
  tar --extract --gunzip && \
  make --directory SDL2_ttf-${sdlTtfVersion}/ cross && \
  rm --force --recursive SDL2_ttf-${sdlTtfVersion}/
# Install dependencies from source packages
RUN glewVersion="2.3.1" && \
  curl --fail --location https://github.com/nigels-com/glew/releases/download/glew-${glewVersion}/glew-${glewVersion}.tgz | \
  tar --extract --gunzip && \
  make --directory glew-${glewVersion}/ SYSTEM=linux-mingw64 install && \
  rm --force --recursive glew-${glewVersion}/ glew.*

# Install apt repository for wine
RUN \
  curl --fail --location https://dl.winehq.org/wine-builds/winehq.key | \
  gpg --dearmor > /etc/apt/keyrings/apt.wine.gpg - && \
  . /etc/os-release && \
  echo "deb [signed-by=/etc/apt/keyrings/apt.wine.gpg] https://dl.winehq.org/wine-builds/ubuntu/ ${UBUNTU_CODENAME} main" > /etc/apt/sources.list.d/wine.list

# Install wine so resulting unit test binaries can be run
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    wine=10.0~repack-12ubuntu1

# Setup compiler and tooling default folders
ENV WINEPATH="${LOCAL_PACKAGE_PATH}bin/;${GCC_RUNTIME_PATH}"


# ----

FROM build-tools AS googletest

RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    cmake=4.2.3-* \
    libgtest-dev=1.17.0-* \
    libgmock-dev=1.17.0-*

# Download, compile, and install Google Test source package
RUN \
  cmake -B/tmp/gtest/ -S/usr/src/googletest/ -DCMAKE_INSTALL_PREFIX="${LOCAL_PACKAGE_PATH}" -DCMAKE_SYSTEM_NAME="Windows" -DCMAKE_BUILD_TYPE=Release -Dgtest_disable_pthreads=ON && \
  cmake --build /tmp/gtest/ && \
  cmake --install /tmp/gtest/ --prefix=/staging/googletest && \
  rm --force --recursive /tmp/gtest/


# ----

FROM dependencies

COPY --link --from=googletest /staging/googletest ${LOCAL_PACKAGE_PATH}

# Set custom variables for build script convenience
# Activate appropriate Toolchain settings
ENV Toolchain=mingw

# Be explicit about the extra flags with the default command
CMD ["make", "--keep-going", "check"]
