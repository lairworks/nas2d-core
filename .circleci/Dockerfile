# See Docker and CircleCI section of makefile in root project folder for usage.

# Depend on local build image
FROM outpostuniverse/nas2d:1.0

USER root
# Install tools needed for primary CircleCI containers
RUN apt-get update && apt-get install -y --no-install-recommends \
    git=1:2.17.1-* \
    ssh=1:7.6p1-* \
    tar=1.29b-* \
    gzip=1.6-* \
    ca-certificates=* \
  && rm -rf /var/lib/apt/lists/*
USER user
