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
Sdl2ModulePackageUrls := https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.3.tar.gz \
	https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.2.tar.gz \
	https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz
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


#### SDL2 core ####

# Less interesting sub-steps
.PHONY: unpack-sdl2 configure-sdl2 compile-sdl2 clean-package-sdl2

Sdl2Package := $(call PackageName,${Sdl2PackageUrl})
Sdl2Folder := $(call PackageFolder,${Sdl2Package})

${Sdl2Package}:
	@echo "\nDownloading SDL2 source package..."
	wget --no-clobber --directory-prefix=${DepBuildDir} ${Sdl2PackageUrl}
download-sdl2: ${Sdl2Package}

${Sdl2Folder}: ${Sdl2Package}
	@echo "\nUnpacking SDL2 source package..."
	tar -C "${DepBuildDir}" -xf ${Sdl2Package}
unpack-sdl2: | ${Sdl2Folder}

${Sdl2Folder}/Makefile: | ${Sdl2Folder}
	@echo "\nConfiguring SDL2 source package..."
	cd ${Sdl2Folder} && ./configure
configure-sdl2: ${Sdl2Folder}/Makefile

${Sdl2Folder}/build/libSDL2.la: ${Sdl2Folder}/Makefile
	@echo "\nCompiling SDL2 source package..."
	make -C ${Sdl2Folder}
compile-sdl2: ${Sdl2Folder}/build/libSDL2.la

sdl2: compile-sdl2

install-sdl2: sdl2
	@echo "\nInstalling SDL2 source package..."
	cd ${Sdl2Folder} && make install

clean-sdl2:
	rm -r ${Sdl2Folder}
clean-package-sdl2:
	rm ${Sdl2Package}
clean-all-sdl2: clean-sdl2 clean-package-sdl2


####

UrlList := $(shell grep -o '^[^\#]*' SourcePackageList.txt)
ArchiveList := $(addprefix ${DepBuildDir}/,$(notdir ${UrlList}))
BuildDirList := $(patsubst %.tar.gz,%,$(patsubst %.tgz,%,$(patsubst %.tar.bz2,%,${ArchiveList})))

# Find path to an SDL2 source folder (used for SDL2 addon modules)
# Note: This allows modules to compile without an already installed SDL2.h
SDL2RootFolder := $(filter ${DepBuildDir}/SDL2-%, ${BuildDirList})
export SDL2_CONFIG := $(abspath ${SDL2RootFolder})/sdl2-config
export INCLUDES := -I${SDL2RootFolder}

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
