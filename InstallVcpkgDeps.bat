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
vcpkg install physfs:%PLATFORM%-windows
vcpkg install glew:%PLATFORM%-windows
vcpkg install SDL2:%PLATFORM%-windows
vcpkg install SDL2-mixer:%PLATFORM%-windows
vcpkg install SDL2-image:%PLATFORM%-windows
vcpkg install SDL2-ttf:%PLATFORM%-windows
vcpkg install gtest:%PLATFORM%-windows
