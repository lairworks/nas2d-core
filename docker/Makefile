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
.PHONY: download-sdl2 sdl2 install-sdl2 clean-sdl2 clean-all-sdl2
.PHONY: download-sdl2-modules sdl2-modules install-sdl2-modules clean-sdl2-modules clean-all-sdl2-modules
.PHONY: download-physfs physfs install-physfs clean-phyfs clean-all-physfs
.PHONY: download-glew glew install-glew clean-glew clean-all-glew

# Source pakcage URLs
Sdl2PackageUrl := https://www.libsdl.org/release/SDL2-2.0.8.tar.gz
Sdl2ImagePackageUrl := https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.3.tar.gz
Sdl2MixerPackageUrl := https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.2.tar.gz
Sdl2TtfPackageUrl := https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz
PhysfsPackageUrl := https://icculus.org/physfs/downloads/physfs-3.0.1.tar.bz2
GlewPackageUrl := https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz

# Note: This variable can be overridden from the command line
# Example:
#   make DepBuildDir=alternatePath
DepBuildDir := ../build/dependencies

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
$(eval $(call SourcePackageInstallRules,physfs,${PhysfsPackageUrl}))
$(eval $(call SourcePackageInstallRules,glew,${GlewPackageUrl}))


####
####

UrlList := $(shell grep -o '^[^\#]*' SourcePackageList.txt)
ArchiveList := $(addprefix ${DepBuildDir}/,$(notdir ${UrlList}))
BuildDirList := $(patsubst %.tar.gz,%,$(patsubst %.tgz,%,$(patsubst %.tar.bz2,%,${ArchiveList})))

# Build rules which do not name a target file
.PHONY: all download unpack configure compile install clean cleanbuild cleanpackages

all: download unpack configure compile

download:
	@echo "\nDownloading source packages..."
	# Strip comments and blank lines before feeding the package list to wget
	grep -o '^[^#]*' SourcePackageList.txt | wget --no-clobber --directory-prefix=${DepBuildDir} --input-file=-

unpack:
	@echo "\nUnpacking source packages..."
	echo -n "${ArchiveList}" | xargs -i -t -d ' ' tar -C "${DepBuildDir}" -xf {}

configure:
	@echo "\nConfiguring source packages..."
	echo -n "${BuildDirList}" | xargs -i -t -d ' ' sh -c "cd {} && { \
			{ [ -f configure ] && ./configure; } || \
			{ [ ! -f CMakeLists.txt ] || cmake ./; }; \
		}"

compile:
	@echo "\nCompiling source packages..."
	echo -n "${BuildDirList}" | xargs -i -t -d ' ' make -C {}

# This target assumes make is run with sudo
install:
	@echo "\nInstalling source packages..."
	echo -n "${BuildDirList}" | xargs -i -t -d ' ' sh -c "cd {} && make install"

# Cleanup rules
clean: cleanbuild cleanpackages
cleanbuild:
	rm -r ${BuildDirList}
cleanpackages:
	rm ${ArchiveList}
