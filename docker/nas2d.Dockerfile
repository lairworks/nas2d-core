# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:18.04

# Install base development tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential=12.4* \
    cmake=3.10.2-* \
  && rm -rf /var/lib/apt/lists/*

# Install Google Test source package
RUN apt-get update && apt-get install -y --no-install-recommends \
    googletest=1.8.0-* \
  && rm -rf /var/lib/apt/lists/*

# Compile and install Google Test
WORKDIR /tmp/gtest/
RUN cmake -DCMAKE_CXX_FLAGS="-std=c++17" /usr/src/googletest/ && \
  make && \
  cp googlemock/gtest/lib*.a /usr/lib && \
  cp googlemock/lib*.a /usr/lib && \
  rm -rf /tmp/gtest/
WORKDIR /

# Install NAS2D specific dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libglew-dev=2.0.0-* \
    libphysfs-dev=3.0.1-* \
    libsdl2-dev=2.0.8+* \
    libsdl2-image-dev=2.0.3+* \
    libsdl2-mixer-dev=2.0.2+* \
    libsdl2-ttf-dev=2.0.14+* \
  && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash user
USER user

VOLUME /code
WORKDIR /code

CMD ["make", "--keep-going", "check"]
