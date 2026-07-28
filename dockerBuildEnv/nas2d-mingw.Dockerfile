# syntax=docker/dockerfile:1.17

# See Docker section of makefile in root project folder for usage commands.


FROM ubuntu:resolute-20260610 AS base-image

# Remove automatic apt package cleanup so a local cache mount can be used
RUN rm /etc/apt/apt.conf.d/docker-clean


# ----

FROM base-image AS build-tools

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

FROM base-image AS wine-apt-repo-key

RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    gnupg=2.4.8-*

ADD --link https://dl.winehq.org/wine-builds/winehq.key /tmp/winehq.key

RUN gpg --output /tmp/apt.wine.gpg --dearmor /tmp/winehq.key


# ----

FROM scratch AS sdl-packages

ARG sdlVersion=2.32.10
ADD --link --unpack=true https://libsdl.org/release/SDL2-devel-${sdlVersion}-mingw.tar.gz /sdl-packages

ARG sdlImageVersion=2.8.12
ADD --link --unpack=true https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-${sdlImageVersion}-mingw.tar.gz /sdl-packages

ARG sdlMixerVersion=2.8.2
ADD --link --unpack=true https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-${sdlMixerVersion}-mingw.tar.gz /sdl-packages

ARG sdlTtfVersion=2.24.0
ADD --link --unpack=true https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-${sdlTtfVersion}-mingw.tar.gz /sdl-packages


# ----

FROM build-tools AS glew

ARG glewVersion=2.3.1
ADD --link --unpack=true https://github.com/nigels-com/glew/releases/download/glew-${glewVersion}/glew-${glewVersion}.tgz /glew-package

RUN make --directory /glew-package/glew-${glewVersion}/ SYSTEM=linux-mingw64
RUN make --directory /glew-package/glew-${glewVersion}/ SYSTEM=linux-mingw64 DESTDIR=/glew install


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

# Compile Google Test from source package and install to staging area
RUN \
  cmake -B/tmp/gtest/ -S/usr/src/googletest/ -DCMAKE_INSTALL_PREFIX="${LOCAL_PACKAGE_PATH}" -DCMAKE_SYSTEM_NAME="Windows" -DCMAKE_BUILD_TYPE=Release -Dgtest_disable_pthreads=ON && \
  cmake --build /tmp/gtest/ && \
  cmake --install /tmp/gtest/ --prefix=/staging/googletest && \
  rm --force --recursive /tmp/gtest/


# ----

FROM build-tools

# Install apt repository for wine
RUN \
  --mount=type=bind,from=wine-apt-repo-key,source=/tmp,target=/wine-key \
  install -D --mode=644 /wine-key/apt.wine.gpg /etc/apt/keyrings/ && \
  . /etc/os-release && \
  echo "deb [signed-by=/etc/apt/keyrings/apt.wine.gpg] https://dl.winehq.org/wine-builds/ubuntu/ ${UBUNTU_CODENAME} main" > /etc/apt/sources.list.d/wine.list

# Install wine so resulting unit test binaries can be run
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    wine=10.0~repack-12ubuntu1

# Set PATH for wine environment
ENV WINEPATH="${LOCAL_PACKAGE_PATH}bin/;${GCC_RUNTIME_PATH}"

# Install SDL libraries from binary packages
RUN \
  --mount=type=bind,from=sdl-packages,source=/sdl-packages,target=/sdl-packages \
  make --directory /sdl-packages/SDL2-*/ cross
RUN \
  --mount=type=bind,from=sdl-packages,source=/sdl-packages,target=/sdl-packages \
  make --directory /sdl-packages/SDL2_image-*/ cross
RUN \
  --mount=type=bind,from=sdl-packages,source=/sdl-packages,target=/sdl-packages \
  make --directory /sdl-packages/SDL2_mixer-*/ cross
RUN \
  --mount=type=bind,from=sdl-packages,source=/sdl-packages,target=/sdl-packages \
  make --directory /sdl-packages/SDL2_ttf-*/ cross

COPY --link --from=glew /glew /

COPY --link --from=googletest /staging/googletest ${LOCAL_PACKAGE_PATH}

# Set custom variables for build script convenience
# Activate appropriate Toolchain settings
ENV Toolchain=mingw

# Be explicit about the extra flags with the default command
CMD ["make", "--keep-going", "check"]
