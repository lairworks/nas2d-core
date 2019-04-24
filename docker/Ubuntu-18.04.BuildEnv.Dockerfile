# See Docker section of makefile in root project folder for usage commands.

FROM ubuntu:18.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++=4:7.3.0-* \
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
