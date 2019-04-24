# See Docker section of makefile in root project folder for usage commands.

FROM outpostuniverse/ubuntu-18.04-gcc:1.0

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
