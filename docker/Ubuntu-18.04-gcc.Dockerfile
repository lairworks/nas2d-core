# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:18.04

# Install base development tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential=12.4* \
    cmake=3.10.2-* \
  && rm -rf /var/lib/apt/lists/*
