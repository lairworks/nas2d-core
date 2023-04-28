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
REM Install dependencies (recursively)
vcpkg install --recurse --triplet %PLATFORM%-windows  glew sdl2 sdl2-image sdl2-ttf sdl2-mixer gtest
