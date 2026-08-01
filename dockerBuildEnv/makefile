# Build rules relating to Docker images

# Capture top level folder before any Makefile includes
# Note: MAKEFILE_LIST's last entry is the last processed Makefile.
#       That should be the current Makefile, assuming no includes
DockerBuildEnvFolder := $(abspath $(dir $(lastword ${MAKEFILE_LIST})))

DockerRunFlags = --volume ${CURDIR}:/code --workdir=/code --rm --tty ${DockerAdditionalFlags}
DockerUserFlags = --user="$(shell id --user):$(shell id --group)"
DockerRepository ?= ghcr.io/lairworks
DockerImageName = ${DockerRepository}/build-env-nas2d-$*
DockerImageNameTagged = ${DockerImageName}:${ImageVersion_$*}
DockerImageNameLatest = ${DockerImageName}:latest

DockerVersionFiles := $(wildcard ${DockerBuildEnvFolder}/*/version.mk)
DockerEnvironments := $(patsubst ${DockerBuildEnvFolder}/%/version.mk,%,${DockerVersionFiles})
include ${DockerVersionFiles}

DockerBuildRules := $(addprefix build-image-, ${DockerEnvironments})
DockerPushRules := $(addprefix push-image-, ${DockerEnvironments})
DockerRunRules := $(addprefix run-image-, ${DockerEnvironments})
DockerDebugRules := $(addprefix debug-image-, ${DockerEnvironments})
DockerDebugRootRules := $(addprefix root-debug-image-, ${DockerEnvironments})

.PHONY: ${DockerBuildRules} ${DockerPushRules} ${DockerRunRules} ${DockerDebugRules} ${DockerDebugRootRules}

${DockerBuildRules}: build-image-%:
	docker build ${DockerBuildEnvFolder}/$*/ --tag ${DockerImageNameTagged} --tag ${DockerImageNameLatest}

${DockerPushRules}: push-image-%:
	docker push ${DockerImageNameTagged}
	docker push ${DockerImageNameLatest}

${DockerRunRules}: run-image-%:
	docker run ${DockerRunFlags} ${DockerUserFlags} ${DockerImageNameTagged}

${DockerDebugRules}: debug-image-%:
	docker run ${DockerRunFlags} --interactive ${DockerUserFlags} ${DockerImageNameTagged} bash

${DockerDebugRootRules}: root-debug-image-%:
	docker run ${DockerRunFlags} --interactive ${DockerImageNameTagged} bash
