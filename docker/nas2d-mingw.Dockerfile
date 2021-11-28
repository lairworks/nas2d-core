# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:22.04

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
# The software-properties-common package is needed for add-apt-repository, used to install wine
# Set DEBIAN_FRONTEND to prevent tzdata package install from prompting for timezone
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    mingw-w64=8.0.0-1 \
    cmake=3.18.4-* \
    make=4.3-* \
    binutils=2.37-* \
    git=1:2.32.0-* \
    ssh=1:8.7p1-2 \
    googletest=1.11.0-3 \
    curl=7.74.0-* \
    tar=1.34+* \
    gzip=1.10-* \
    bzip2=1.0.8-* \
    gnupg=2.2.27-* \
    software-properties-common=0.99.16 \
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
RUN curl -L https://dl.winehq.org/wine-builds/winehq.key | apt-key add - && \
  add-apt-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ impish main' && \
  dpkg --add-architecture i386 && \
  apt-get update && apt-get install -y --no-install-recommends \
    wine-stable-amd64=6.0.2~impish-1 \
    wine-stable-i386=6.0.2~impish-1 \
    wine-stable=6.0.2~impish-1 \
    winehq-stable=6.0.2~impish-1 \
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
RUN curl https://libsdl.org/release/SDL2-devel-2.0.16-mingw.tar.gz | tar -xz && \
  make -C SDL2-2.0.16/ cross && \
  rm -rf SDL2-2.0.16/
RUN curl https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz | tar -xz && \
  make -C SDL2_image-2.0.5/ cross && \
  rm -rf SDL2_image-2.0.5/
RUN curl https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz | tar -xz && \
  make -C SDL2_mixer-2.0.4/ cross && \
  rm -rf SDL2_mixer-2.0.4/
RUN curl https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-mingw.tar.gz | tar -xz && \
  make -C SDL2_ttf-2.0.15/ cross && \
  rm -rf SDL2_ttf-2.0.15/
# Install dependencies from source packages
RUN curl --location https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz | tar -xz && \
  make -C glew-2.1.0/ SYSTEM=linux-mingw-w64 CC="${CC64}" LD="${LD64}" LDFLAGS.EXTRA=-L"/usr/${ARCH64}/lib/" GLEW_DEST="${INSTALL64}" install && \
  make -C glew-2.1.0/ distclean && \
  make -C glew-2.1.0/ SYSTEM=linux-mingw-w64 CC="${CC32}" LD="${LD32}" LDFLAGS.EXTRA=-L"/usr/${ARCH32}/lib/" GLEW_DEST="${INSTALL32}" install && \
  rm -rf glew-2.1.0/ glew.*
RUN curl https://icculus.org/physfs/downloads/physfs-3.0.2.tar.bz2 | tar -xj && \
  cmake -H"physfs-3.0.2/" -B"${ARCH64}" -DCMAKE_INSTALL_PREFIX="${INSTALL64}" -DCMAKE_CXX_COMPILER="${CXX64}" -DCMAKE_C_COMPILER="${CC64}" -DCMAKE_SYSTEM_NAME="${TARGET_OS}" && \
  make -C "${ARCH64}" install && \
  cmake -H"physfs-3.0.2/" -B"${ARCH32}" -DCMAKE_INSTALL_PREFIX="${INSTALL32}" -DCMAKE_CXX_COMPILER="${CXX32}" -DCMAKE_C_COMPILER="${CC32}" -DCMAKE_SYSTEM_NAME="${TARGET_OS}" && \
  make -C "${ARCH32}" install && \
  rm -rf physfs-3.0.2/ "${ARCH64}" "${ARCH32}"

# Custom variables for install locations
ENV INCLUDE64=${INSTALL64}include/
ENV INCLUDE32=${INSTALL32}include/
ENV LIB64=${INSTALL64}lib;/usr/${ARCH64}/lib
ENV LIB32=${INSTALL32}lib;/usr/${ARCH32}/lib
ENV BIN64=${INSTALL64}bin/
ENV BIN32=${INSTALL32}bin/
ENV PATH64="${PATH}:${BIN64}"
ENV PATH32="${PATH}:${BIN32}"
ENV WINEPATH64=${BIN64};/usr/lib/gcc/${ARCH64}/10-win32/
ENV WINEPATH32=${BIN32};/usr/lib/gcc/${ARCH32}/10-win32/

# Setup compiler and tooling default folders
ENV CPLUS_INCLUDE_PATH="${INCLUDE64}"
ENV LIBRARY_PATH="${LIB64}"
ENV PATH="${PATH64}"
ENV WINEPATH="${WINEPATH64}"

# Set default compiler
ENV CXX=${CXX64}
ENV  CC=${CC64}

RUN useradd -m -s /bin/bash user
USER user

VOLUME /code
WORKDIR /code

# Pre-setup Wine to save startup time later
RUN wineboot

# Set default extra C pre-processor flags
# This makes proper rebuilding easier in a debug session
ENV CPPFLAGS_EXTRA=-D"GLEW_STATIC"
# Disable warnings for redundant declarations of intrinsics, triggered by SDL2
ENV WARN_EXTRA=-Wno-redundant-decls

# Be explicit about the extra flags with the default command
CMD ["make", "--keep-going", "check"]
