@echo off

set "Configuration=%~1"
set "Platform=%~2"
set "TargetPath=%~3"

set "ProjectName=NAS2D"
set "OutputFolder=.build\"
set "PackageFolder=%OutputFolder%Package\"

if "%TargetPath%" == "" (set "TargetPath=%OutputFolder%%Configuration%_%Platform%\%ProjectName%\%ProjectName%.lib")

for /f %%i in ('git describe --tags --dirty') do set Version=%%i

set "Config=Windows.%Platform%.%Configuration%"
set "PackageName=nas2d-%Version%-%Config%.zip"

cd /D "%~dp0"
xcopy /y /q "%TargetPath%" "%PackageFolder%"
xcopy /s /y /q NAS2D\*.h "%PackageFolder%include\"
cd "%PackageFolder%"

powershell -Command "Compress-Archive -Path 'NAS2D.lib', 'include/' -DestinationPath '%PackageName%' -Force"
move /Y *.zip ..
