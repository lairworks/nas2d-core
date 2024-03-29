# Makefile used to download, build, and install source dependencies on Linux
# Example:
#   make sdl2
#   sudo make-install-sdl2
#   make clean-sdl2
#
# Example:
#   make sdl2-modules && make install-sdl2-modules && make-clean-all-sdl2-modules

# Build rules (which do not name a target file)
.PHONY: all download-all
.PHONY: download-sdl2-modules compile-sdl2-modules install-sdl2-modules
.PHONY: clean-sdl2-modules clean-all-sdl2-modules

all: install-sdl2 install-sdl2-modules install-glew
download-all: download-sdl2 download-sdl2-modules download-glew
download-sdl2-modules: download-sdl2-image download-sdl2-mixer download-sdl2-ttf
compile-sdl2-modules: compile-sdl2-image compile-sdl2-mixer compile-sdl2-ttf
install-sdl2-modules: install-sdl2-image install-sdl2-mixer install-sdl2-ttf
clean-sdl2-modules: clean-sdl2-image clean-sdl2-mixer clean-sdl2-ttf
clean-all-sdl2-modules: clean-all-sdl2-image clean-all-sdl2-mixer clean-all-sdl2-ttf

# Source pakcage URLs
Sdl2PackageUrl := https://www.libsdl.org/release/SDL2-2.0.8.tar.gz
Sdl2ImagePackageUrl := https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.3.tar.gz
Sdl2MixerPackageUrl := https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.2.tar.gz
Sdl2TtfPackageUrl := https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz
GlewPackageUrl := https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz

# Note: This variable can be overridden from the command line
# Example:
#   make DepBuildDir=alternatePath
DepBuildDir := ../.build/dependencies

# Note: Functions are not simply expanded (Must use "=" instead of ":=" )
# Function to convert URL to package name, and package name to folder name
PackageName = $(addprefix ${DepBuildDir}/,$(notdir $(1)))
PackageFolder = $(patsubst %.tar.gz,%,$(patsubst %.tgz,%,$(patsubst %.tar.bz2,%,$(1))))


#### Generic Source Install Rules ####

define SourcePackageInstallRules =
.PHONY: download-$(1) unpack-$(1) configure-$(1) compile-$(1) $(1) install-$(1)
.PHONY: clean-$(1) clean-package-$(1) clean-all-$(1)

SourcePackage_$(1) := $$(call PackageName,$(2))
SourceFolder_$(1) := $$(call PackageFolder,$${SourcePackage_$(1)})

$${SourcePackage_$(1)}:
	@echo "\nDownloading $(1) source package..."
	wget -4 --no-clobber --directory-prefix=${DepBuildDir} $(2)
download-$(1): $${SourcePackage_$(1)}

$${SourceFolder_$(1)}: $${SourcePackage_$(1)}
	@echo "\nUnpacking $(1) source package..."
	tar -C "${DepBuildDir}" -xf $${SourcePackage_$(1)}
unpack-$(1): | $${SourceFolder_$(1)}

$${SourceFolder_$(1)}/Makefile: | $${SourceFolder_$(1)}
	@echo "\nConfiguring $(1) source package..."
	cd $${SourceFolder_$(1)} && { \
			{ [ -f configure ] && ./configure; } || \
			{ [ ! -f CMakeLists.txt ] || cmake ./; }; \
		}
configure-$(1): $${SourceFolder_$(1)}/Makefile

compile-$(1): $${SourceFolder_$(1)}/Makefile
	@echo "\nCompiling $(1) source package..."
	make -C $${SourceFolder_$(1)}
$(1): compile-$(1)

install-$(1): $(1)
	@echo "\nInstalling $(1) source package..."
	cd $${SourceFolder_$(1)} && make install

clean-$(1):
	rm -r $${SourceFolder_$(1)}
clean-package-$(1):
	rm $${SourcePackage_$(1)}
clean-all-$(1): clean-$(1) clean-package-$(1)
endef


#### Setup generic rules for each package ####

$(eval $(call SourcePackageInstallRules,sdl2,${Sdl2PackageUrl}))
$(eval $(call SourcePackageInstallRules,sdl2-image,${Sdl2ImagePackageUrl}))
$(eval $(call SourcePackageInstallRules,sdl2-mixer,${Sdl2MixerPackageUrl}))
$(eval $(call SourcePackageInstallRules,sdl2-ttf,${Sdl2TtfPackageUrl}))
$(eval $(call SourcePackageInstallRules,glew,${GlewPackageUrl}))
