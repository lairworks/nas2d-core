# Makefile used to help download, build, and install dependencies on Linux

# Note: This variable can be overridden from the command line
# Example:
#   make DepBuildDir=alternatePath
DepBuildDir := dependencies

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
