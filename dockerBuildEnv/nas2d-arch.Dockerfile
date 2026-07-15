# See Docker section of makefile in root project folder for usage commands.

FROM archlinux:base-20260628.0.549485

# Install base development tools
# Includes tools to build download, unpack, and build source packages
# Includes tools needed for primary CircleCI containers
RUN \
  --mount=type=cache,target=/var/cache/pacman/pkg,sharing=locked \
  pacman --sync --refresh --noconfirm \
    gcc \
    make \
    gtest \
    git \
    openssh \
    tar \
    gzip \
    ca-certificates

RUN \
  --mount=type=cache,target=/var/cache/pacman/pkg,sharing=locked \
  pacman --sync --refresh --noconfirm \
    glew \
    sdl2 \
    sdl2_image \
    sdl2_mixer \
    sdl2_ttf

RUN useradd --create-home user

# Set custom variables for build script convenience
# Activate appropriate Toolchain settings
ENV Toolchain=gcc

CMD ["make", "--keep-going", "check"]
