@echo off
echo "This batch file will install dependencies on Windows with vcpkg"

vcpkg version
if %ERRORLEVEL% NEQ 0 (
  echo "The command `vcpkg` is not installed"
  exit /b
)

if not defined PLATFORM (
  set PLATFORM=x86
  call :Install
  set PLATFORM=x64
  call :Install
  exit /b
)

:Install
vcpkg install --triplet %PLATFORM%-windows  physfs glew SDL2 SDL2-image SDL2-ttf SDL2-mixer[dynamic-load,libflac,libmodplug,libvorbis,opusfile] gtest
