# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:resolute-20260610

# Remove automatic apt package cleanup so a local cache mount can be used
RUN rm /etc/apt/apt.conf.d/docker-clean

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
# The lsb-release package is used to install wine
# Set DEBIAN_FRONTEND to prevent tzdata package install from prompting for timezone
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    g++-mingw-w64-x86-64-win32=13.2.0-* \
    make=4.4.1-* \
    cmake=4.2.3-* \
    libgtest-dev=1.17.0-* \
    libgmock-dev=1.17.0-* \
    git=1:2.53.0-* \
    ssh=1:10.2p1-* \
    curl=8.18.0-* \
    gnupg=2.4.8-* \
    lsb-release=12.1-* \
    tar=1.35+* \
    gzip=1.14-* \
    ca-certificates=*

# Set architecture short names
ENV ARCH64=x86_64-w64-mingw32
# Set compiler short names
ENV CXX64=${ARCH64}-g++
ENV  CC64=${ARCH64}-gcc
ENV  LD64=${ARCH64}-ld
ENV  AR64=${ARCH64}-ar
ENV  STRIP64=${ARCH64}-strip

# Set default compiler
ENV CXX=${CXX64}
ENV  CC=${CC64}
ENV  LD=${LD64}
ENV  AR=${AR64}
ENV  STRIP=${STRIP64}

# Install apt repository for wine
RUN \
  curl -L https://dl.winehq.org/wine-builds/winehq.key | gpg --dearmor > /etc/apt/keyrings/apt.wine.gpg - && \
  echo "deb [signed-by=/etc/apt/keyrings/apt.wine.gpg] https://dl.winehq.org/wine-builds/ubuntu/ $(lsb_release -cs) main" > /etc/apt/sources.list.d/wine.list

# Install wine so resulting unit test binaries can be run
RUN \
  --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
  apt-get update && \
  apt-get install -y --no-install-recommends \
    wine=10.0~repack-12ubuntu1

# Set default install location for custom packages
ENV INSTALL_PREFIX=/usr/local/
ENV INSTALL64=${INSTALL_PREFIX}${ARCH64}/

# Custom variables for install locations
ENV INCLUDE64=${INSTALL64}include/
ENV LIB64=${INSTALL64}lib;/usr/${ARCH64}/lib
ENV BIN64=${INSTALL64}bin/
ENV PATH64="${PATH}:${BIN64}"
ENV WINEPATH64=${BIN64};/usr/lib/gcc/${ARCH64}/13-win32/

# Setup compiler and tooling default folders
ENV CPLUS_INCLUDE_PATH="${INCLUDE64}"
ENV LIBRARY_PATH="${LIB64}"
ENV PATH="${PATH64}"
ENV WINEPATH="${WINEPATH64}"

# Create directories for local install of libraries
RUN mkdir --parents "${INSTALL64}"

# Download, compile, and install Google Test source package
RUN \
  mkdir --parents /tmp/gtest/ && \
  cd /tmp/gtest/ && \
  cmake -H/usr/src/googletest/ -B"${ARCH64}" -DCMAKE_CXX_FLAGS="-std=c++20" -DCMAKE_SYSTEM_NAME="Windows" -Dgtest_disable_pthreads=ON && make -C "${ARCH64}" && \
  cmake -H/usr/src/googletest/ -B"${ARCH64}" -DCMAKE_CXX_FLAGS="-std=c++20" -DCMAKE_SYSTEM_NAME="Windows" -Dgtest_disable_pthreads=ON -DBUILD_SHARED_LIBS=ON && make -C "${ARCH64}" && \
  cp --parents -r \
    "${ARCH64}/bin/" \
    "${ARCH64}/lib/" \
    "${INSTALL_PREFIX}" && \
  mkdir -p \
    "${INSTALL_PREFIX}share/mingw-w64/" \
    "${INSTALL64}include/" && \
  cp -r \
    /usr/src/googletest/googletest/include/ \
    /usr/src/googletest/googlemock/include/ \
    "${INSTALL_PREFIX}share/mingw-w64/" && \
  ln -sf "${INSTALL_PREFIX}share/mingw-w64/include/gtest/" "${INSTALL64}include/" && \
  ln -sf "${INSTALL_PREFIX}share/mingw-w64/include/gmock/" "${INSTALL64}include/" && \
  cp --parents -r \
    /usr/src/googletest/CMakeLists.txt \
    /usr/src/googletest/googletest/ \
    /usr/src/googletest/googlemock/ \
    "${INSTALL_PREFIX}src/" && \
  rm -rf /tmp/gtest/

# Install NAS2D specific dependencies
WORKDIR /tmp/
# Install SDL libraries from binary packages
RUN sdlVersion="2.32.10" && \
  curl https://libsdl.org/release/SDL2-devel-${sdlVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2-${sdlVersion}/ cross && \
  rm -rf SDL2-${sdlVersion}/
RUN sdlImageVersion="2.8.12" && \
  curl https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-${sdlImageVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2_image-${sdlImageVersion}/ cross && \
  rm -rf SDL2_image-${sdlImageVersion}/
RUN sdlMixerVersion="2.8.2" && \
  curl https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-${sdlMixerVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2_mixer-${sdlMixerVersion}/ cross && \
  rm -rf SDL2_mixer-${sdlMixerVersion}/
RUN sdlTtfVersion="2.24.0" && \
  curl https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-${sdlTtfVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2_ttf-${sdlTtfVersion}/ cross && \
  rm -rf SDL2_ttf-${sdlTtfVersion}/
# Install dependencies from source packages
RUN glewVersion="2.3.1" && \
  curl --location https://github.com/nigels-com/glew/releases/download/glew-${glewVersion}/glew-${glewVersion}.tgz | tar -xz && \
  make -C glew-${glewVersion}/ SYSTEM=linux-mingw64 WARN="-Wno-cast-function-type" LDFLAGS.EXTRA=-L"/usr/${ARCH64}/lib/" GLEW_DEST="${INSTALL64}" install && \
  rm -rf glew-${glewVersion}/ glew.*

# Set custom variables for build script convenience
# Activate appropriate Toolchain settings
ENV Toolchain=mingw
# Set a library search path to use during linking
ENV LDFLAGS_EXTRA="-L/usr/local/${ARCH64}/lib"

# Be explicit about the extra flags with the default command
CMD ["make", "--keep-going", "check"]
