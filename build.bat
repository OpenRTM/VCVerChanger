@echo off
@rem ============================================================
@rem VCVerChanger for Windows build batch
@rem
@rem Build with vc2015. mfc140.dll is required.
@rem
@rem ============================================================

echo ARCH %ARCH%

if "%ARCH%" == "x86_64" (
  set BUILD_ARCH=amd64
  set PLATFORM=x64
)

call C:\"Program Files (x86)"\"Microsoft Visual Studio 14.0"\VC\vcvarsall.bat %BUILD_ARCH%
set VCTOOLSET=14.0
set PLATFORMTOOL=/p:PlatformToolset=v140
set OPT=/M:4 /toolsversion:%VCTOOLSET% %PLATFORMTOOL% /p:platform=%PLATFORM%
set SLN=VCVerChanger.sln
set LOG=/fileLogger /flp:logfile=debug.log /v:diag 

msbuild /t:rebuild /p:configuration=release %OPT% %SLN%


