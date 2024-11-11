# Build rules relating to Docker images

# Capture top level folder before any Makefile includes
# Note: MAKEFILE_LIST's last entry is the last processed Makefile.
#       That should be the current Makefile, assuming no includes
DockerFolder := $(abspath $(dir $(lastword ${MAKEFILE_LIST})))
TopLevelFolder := $(abspath $(DockerFolder)/..)

DockerRunFlags := --volume ${TopLevelFolder}:/code --workdir=/code --rm --tty
DockerUserFlags = --user="$(shell id --user):$(shell id --group)"
DockerRepository ?= outpostuniverse

include $(wildcard $(DockerFolder)/nas2d-*.version.mk)

DockerFileName = ${DockerFolder}/nas2d-$*.Dockerfile

DockerImageName = ${DockerRepository}/nas2d-$*
DockerImageNameTagged = ${DockerImageName}:${ImageVersion_$*}
DockerImageNameLatest = ${DockerImageName}:latest

DockerBuildRules := build-image-gcc build-image-clang build-image-mingw build-image-arch
DockerPushRules := push-image-gcc push-image-clang push-image-mingw push-image-arch
DockerRunRules := run-image-gcc run-image-clang run-image-mingw run-image-arch
DockerDebugRules := debug-image-gcc debug-image-clang debug-image-mingw debug-image-arch
DockerDebugRootRules := root-debug-image-gcc root-debug-image-clang root-debug-image-mingw root-debug-image-arch

.PHONY: ${DockerBuildRules} ${DockerPushRules} ${DockerRunRules} ${DockerDebugRules} ${DockerDebugRootRules}

${DockerBuildRules}: build-image-%:
	docker build ${DockerFolder}/ --file ${DockerFileName} --tag ${DockerImageNameTagged} --tag ${DockerImageNameLatest}

${DockerPushRules}: push-image-%:
	docker push ${DockerImageNameTagged}
	docker push ${DockerImageNameLatest}

${DockerRunRules}: run-image-%:
	docker run ${DockerRunFlags} ${DockerUserFlags} ${DockerImageNameTagged}

${DockerDebugRules}: debug-image-%:
	docker run ${DockerRunFlags} --interactive ${DockerUserFlags} ${DockerImageNameTagged} bash

${DockerDebugRootRules}: root-debug-image-%:
	docker run ${DockerRunFlags} --interactive ${DockerImageNameTagged} bash
