# Build rules relating to Docker images

# Capture top level folder before any Makefile includes
# Note: MAKEFILE_LIST's last entry is the last processed Makefile.
#       That should be the current Makefile, assuming no includes
DockerFolder := $(abspath $(dir $(lastword ${MAKEFILE_LIST})))
TopLevelFolder := $(abspath $(DockerFolder)/..)

DockerRunFlags := --volume ${TopLevelFolder}:/code --workdir=/code --rm --tty
DockerUserFlags = --user="$(shell id --user):$(shell id --group)"
DockerRepository := outpostuniverse

ImageVersion_gcc := 1.5
ImageVersion_clang := 1.4
ImageVersion_mingw := 1.10
ImageVersion_arch := 1.4

DockerFileName = ${DockerFolder}/nas2d-$*.Dockerfile

DockerImageName = ${DockerRepository}/nas2d-$*:${ImageVersion_$*}
DockerImageNameLatest = ${DockerRepository}/nas2d-$*:latest

DockerBuildRules := build-image-gcc build-image-clang build-image-mingw build-image-arch
DockerPushRules := push-image-gcc push-image-clang push-image-mingw push-image-arch
DockerRunRules := run-image-gcc run-image-clang run-image-mingw run-image-arch
DockerDebugRules := debug-image-gcc debug-image-clang debug-image-mingw debug-image-arch
DockerDebugRootRules := root-debug-image-gcc root-debug-image-clang root-debug-image-mingw root-debug-image-arch

.PHONY: ${DockerBuildRules} ${DockerPushRules} ${DockerRunRules} ${DockerDebugRules} ${DockerDebugRootRules}

${DockerBuildRules}: build-image-%:
	docker build ${DockerFolder}/ --file ${DockerFileName} --tag ${DockerImageName} --tag ${DockerImageNameLatest}

${DockerPushRules}: push-image-%:
	docker push ${DockerImageName}
	docker push ${DockerImageNameLatest}

${DockerRunRules}: run-image-%:
	docker run ${DockerRunFlags} ${DockerUserFlags} ${DockerImageName}

${DockerDebugRules}: debug-image-%:
	docker run ${DockerRunFlags} --interactive ${DockerUserFlags} ${DockerImageName} bash

${DockerDebugRootRules}: root-debug-image-%:
	docker run ${DockerRunFlags} --interactive ${DockerImageName} bash
