# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:24.04

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
# The lsb-release package is used to install wine
# Set DEBIAN_FRONTEND to prevent tzdata package install from prompting for timezone
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    mingw-w64=11.0.1-* \
    cmake=3.28.3-* \
    make=4.3-* \
    binutils=2.42-* \
    libgtest-dev=1.14.0-* \
    libgmock-dev=1.14.0-* \
    git=1:2.43.0-* \
    ssh=1:9.6p1-* \
    curl=8.5.0-* \
    tar=1.35+* \
    gzip=1.12-* \
    bzip2=1.0.8-* \
    gnupg=2.4.4-* \
    lsb-release=12.0-* \
    ca-certificates=* \
  && rm -rf /var/lib/apt/lists/*

# Set custom variables for build script convenience
# Set default target OS
ENV TARGET_OS=Windows
# Set architecture short names
ENV ARCH64=x86_64-w64-mingw32
ENV ARCH32=i686-w64-mingw32
# Set compiler short names
ENV CXX64=${ARCH64}-g++
ENV  CC64=${ARCH64}-gcc
ENV  LD64=${ARCH64}-ld
ENV CXX32=${ARCH32}-g++
ENV  CC32=${ARCH32}-gcc
ENV  LD32=${ARCH32}-ld

# Install wine so resulting unit test binaries can be run
RUN curl -L https://dl.winehq.org/wine-builds/winehq.key | gpg --dearmor > /etc/apt/keyrings/apt.wine.gpg - && \
  echo "deb [signed-by=/etc/apt/keyrings/apt.wine.gpg] https://dl.winehq.org/wine-builds/ubuntu/ $(lsb_release -cs) main" > /etc/apt/sources.list.d/wine.list && \
  dpkg --add-architecture i386 && \
  apt-get update && apt-get install -y --no-install-recommends \
    wine=9.0~repack-4build3 \
    wine64=9.0~repack-4build3 \
    wine32:i386=9.0~repack-4build3 \
  && rm -rf /var/lib/apt/lists/*

# Set default install location for custom packages
ENV INSTALL_PREFIX=/usr/local/
ENV INSTALL64=${INSTALL_PREFIX}${ARCH64}/
ENV INSTALL32=${INSTALL_PREFIX}${ARCH32}/

# Create directories for local install of libraries
RUN mkdir --parents "${INSTALL64}" "${INSTALL32}"

# Download, compile, and install Google Test source package
WORKDIR /tmp/gtest/
RUN \
  cmake -H/usr/src/googletest/ -B"${ARCH64}" -DCMAKE_CXX_COMPILER="${CXX64}" -DCMAKE_C_COMPILER="${CC64}" -DCMAKE_CXX_FLAGS="-std=c++20" -DCMAKE_SYSTEM_NAME="${TARGET_OS}" -Dgtest_disable_pthreads=ON && make -C "${ARCH64}" && \
  cmake -H/usr/src/googletest/ -B"${ARCH64}" -DCMAKE_CXX_COMPILER="${CXX64}" -DCMAKE_C_COMPILER="${CC64}" -DCMAKE_CXX_FLAGS="-std=c++20" -DCMAKE_SYSTEM_NAME="${TARGET_OS}" -Dgtest_disable_pthreads=ON -DBUILD_SHARED_LIBS=ON && make -C "${ARCH64}" && \
  cmake -H/usr/src/googletest/ -B"${ARCH32}" -DCMAKE_CXX_COMPILER="${CXX32}" -DCMAKE_C_COMPILER="${CC32}" -DCMAKE_CXX_FLAGS="-std=c++20" -DCMAKE_SYSTEM_NAME="${TARGET_OS}" -Dgtest_disable_pthreads=ON && make -C "${ARCH32}" && \
  cmake -H/usr/src/googletest/ -B"${ARCH32}" -DCMAKE_CXX_COMPILER="${CXX32}" -DCMAKE_C_COMPILER="${CC32}" -DCMAKE_CXX_FLAGS="-std=c++20" -DCMAKE_SYSTEM_NAME="${TARGET_OS}" -Dgtest_disable_pthreads=ON -DBUILD_SHARED_LIBS=ON && make -C "${ARCH32}" && \
  cp --parents -r \
    "${ARCH64}/bin/" \
    "${ARCH64}/lib/" \
    "${ARCH32}/bin/" \
    "${ARCH32}/lib/" \
    "${INSTALL_PREFIX}" && \
  mkdir -p \
    "${INSTALL_PREFIX}share/mingw-w64/" \
    "${INSTALL64}include/" \
    "${INSTALL32}include/" && \
  cp -r \
    /usr/src/googletest/googletest/include/ \
    /usr/src/googletest/googlemock/include/ \
    "${INSTALL_PREFIX}share/mingw-w64/" && \
  ln -sf "${INSTALL_PREFIX}share/mingw-w64/include/gtest/" "${INSTALL64}include/" && \
  ln -sf "${INSTALL_PREFIX}share/mingw-w64/include/gmock/" "${INSTALL64}include/" && \
  ln -sf "${INSTALL_PREFIX}share/mingw-w64/include/gtest/" "${INSTALL32}include/" && \
  ln -sf "${INSTALL_PREFIX}share/mingw-w64/include/gmock/" "${INSTALL32}include/" && \
  cp --parents -r \
    /usr/src/googletest/CMakeLists.txt \
    /usr/src/googletest/googletest/ \
    /usr/src/googletest/googlemock/ \
    "${INSTALL_PREFIX}src/" && \
  rm -rf /tmp/gtest/

# Install NAS2D specific dependencies
WORKDIR /tmp/
# Install SDL libraries from binary packages
RUN sdlVersion="2.32.0" && \
  curl https://libsdl.org/release/SDL2-devel-${sdlVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2-${sdlVersion}/ cross && \
  rm -rf SDL2-${sdlVersion}/
RUN sdlImageVersion="2.8.5" && \
  curl https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-${sdlImageVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2_image-${sdlImageVersion}/ cross && \
  rm -rf SDL2_image-${sdlImageVersion}/
RUN sdlMixerVersion="2.8.1" && \
  curl https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-${sdlMixerVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2_mixer-${sdlMixerVersion}/ cross && \
  rm -rf SDL2_mixer-${sdlMixerVersion}/
RUN sdlTtfVersion="2.24.0" && \
  curl https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-${sdlTtfVersion}-mingw.tar.gz | tar -xz && \
  make -C SDL2_ttf-${sdlTtfVersion}/ cross && \
  rm -rf SDL2_ttf-${sdlTtfVersion}/
# Install dependencies from source packages
RUN glewVersion="2.2.0" && \
  curl --location https://github.com/nigels-com/glew/releases/download/glew-${glewVersion}/glew-${glewVersion}.tgz | tar -xz && \
  make -C glew-${glewVersion}/ SYSTEM=linux-mingw64 CC="${CC64}" WARN="-Wno-cast-function-type" LD="${LD64}" LDFLAGS.EXTRA=-L"/usr/${ARCH64}/lib/" GLEW_DEST="${INSTALL64}" install && \
  make -C glew-${glewVersion}/ distclean && \
  make -C glew-${glewVersion}/ SYSTEM=linux-mingw64 CC="${CC32}" WARN="-Wno-cast-function-type" LD="${LD32}" LDFLAGS.EXTRA=-L"/usr/${ARCH32}/lib/" GLEW_DEST="${INSTALL32}" install && \
  rm -rf glew-${glewVersion}/ glew.*

# Custom variables for install locations
ENV INCLUDE64=${INSTALL64}include/
ENV INCLUDE32=${INSTALL32}include/
ENV LIB64=${INSTALL64}lib;/usr/${ARCH64}/lib
ENV LIB32=${INSTALL32}lib;/usr/${ARCH32}/lib
ENV BIN64=${INSTALL64}bin/
ENV BIN32=${INSTALL32}bin/
ENV PATH64="${PATH}:${BIN64}"
ENV PATH32="${PATH}:${BIN32}"
ENV WINEPATH64=${BIN64};/usr/lib/gcc/${ARCH64}/13-win32/
ENV WINEPATH32=${BIN32};/usr/lib/gcc/${ARCH32}/13-win32/

# Setup compiler and tooling default folders
ENV CPLUS_INCLUDE_PATH="${INCLUDE64}"
ENV LIBRARY_PATH="${LIB64}"
ENV PATH="${PATH64}"
ENV WINEPATH="${WINEPATH64}"

# Set default compiler
ENV CXX=${CXX64}
ENV  CC=${CC64}

# Cache the result of `wineboot` for faster startup (or don't for smaller images)
# USER user

# Pre-setup Wine to save startup time later
# RUN wineboot

# USER root

# Set default extra C pre-processor flags
# This makes proper rebuilding easier in a debug session
ENV CPPFLAGS_EXTRA=-D"GLEW_STATIC"
# Disable warnings for redundant declarations of intrinsics, triggered by SDL2
ENV WARN_EXTRA=-Wno-redundant-decls
# Set a library search path to make rebuilding easier in a debug sessions
ENV LDFLAGS_EXTRA="-L/usr/local/x86_64-w64-mingw32/lib"

# Be explicit about the extra flags with the default command
CMD ["make", "--keep-going", "check"]
