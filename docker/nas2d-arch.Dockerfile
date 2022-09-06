# See Docker section of makefile in root project folder for usage commands.

FROM archlinux:base-20220828.0.78480

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
RUN pacman --sync --refresh --noconfirm \
    gcc \
    make \
    gtest \
    git \
    openssh \
    tar \
    gzip \
    ca-certificates \
  && rm -rf /var/cache/pacman/pkg

RUN pacman --sync --refresh --noconfirm \
    glew \
    sdl2 \
    sdl2_image \
    sdl2_mixer \
    sdl2_ttf \
  && rm -rf /var/cache/pacman/pkg

CMD ["make", "--keep-going", "check"]
