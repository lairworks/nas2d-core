# Based on Ubuntu 18.04, with tools needed to run on CircleCI, and dependencies for NAS2D

# Build from the root project folder with:
#   docker build .circleci/ --tag outpostuniverse/ubuntu-18.04-gcc-sdl2-physfs-circleci
# Push image to DockerHub with:
#   docker login
#   docker push outpostuniverse/ubuntu-18.04-gcc-sdl2-physfs-circleci

# Run locally using the CircleCI command line interface:
#   circleci build
# Validate .circleci/config.yml file with:
#   circleci config validate

# Depend on local build image
FROM outpostuniverse/ubuntu-18.04-gcc-sdl2-physfs

USER root
# Install CircleCI tools needed for primary CircleCI containers
RUN apt-get update && apt-get install -y --no-install-recommends \
    git=1:2.17.1-* \
    ssh=1:7.6p1-* \
    tar=1.29b-* \
    gzip=1.6-* \
    ca-certificates=* \
  && rm -rf /var/lib/apt/lists/*
USER user

# Code linter complains about missing MAINTAINER tag.
# The MAINTAINER tag was deprecated in Docker 1.13. The alternative is to use a LABEL tag.
# This is probably better off not being in the Dockerfile at all though for open source projects.
# Specifying maintainers in source files is known to discourage other people from making updates.
MAINTAINER "WhoEverWantsToEdit <anybody@anywhere.com>"
