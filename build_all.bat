@echo off
rem ============================================================
rem VCVerChanger for Windows build batch
rem
rem Build with vc2010. mfc100.dll is required.
rem
rem ============================================================
set OUTPUT_DIR=VCVerChanger-binary
set ZIP_FILE=%OUTPUT_DIR%.zip

rem ------------------------------------------------------------
rem build (32bit)
rem ------------------------------------------------------------
set ARCH=x86
mkdir %OUTPUT_DIR%\%ARCH%

call build.bat
xcopy /f Release\VCVerChanger.exe %OUTPUT_DIR%\%ARCH%

rem ------------------------------------------------------------
rem build (64bit)
rem ------------------------------------------------------------
set ARCH=x86_64
mkdir %OUTPUT_DIR%\%ARCH%

call build.bat
xcopy /f x64\Release\VCVerChanger.exe %OUTPUT_DIR%\%ARCH%

rem ------------------------------------------------------------
rem make zip package
rem ------------------------------------------------------------
if exist %ZIP_FILE% del %ZIP_FILE%
powershell Compress-Archive .\%OUTPUT_DIR% -DestinationPath .\%ZIP_FILE%
@echo on
