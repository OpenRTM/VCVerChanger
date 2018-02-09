@echo off
@rem ================================================================
@rem Build batch for VCVerChanger Merge Module
@rem
@rem @author Nobu Kawauchi
@rem                Copyright (C) 2017 n-ando All Rights Reserved
@rem
@rem output msm files under MergeModules directory:
@rem  ex. VCVerChanger_x86.msm
@rem ================================================================

@if "%1" == "" exit/b
@if "%2" == "" exit/b
@set SVN_USER=%1
@set SVN_USER_PASSWD=%2

if not defined INSTALL_VERSION set INSTALL_VERSION=1.2.0
if not defined REPOSITORY set REPOSITORY=openrtm.org
if not defined BASE_URL set BASE_URL=https://%REPOSITORY%/pub/Windows/OpenRTM-aist/msi-buildtools/%INSTALL_VERSION%
if not defined BINARY set BINARY=VCVerChanger-binary.zip
if not defined TOOL_VERSION set TOOL_VERSION=1.0.0
if not defined ARCH set ARCH=x86

@set TARGET_VERSION=%INSTALL_VERSION%
@set PKG_NAME=VCVerChanger
@set MSM_VER=%PKG_NAME%%TOOL_VERSION:.=%
@set SHELL=C:\cygwin64\bin\

@rem ------------------------------------------------------------
@rem set GUID file path
set PWD=%CD%
set GUIDS_FILE=%PWD%\VCVerChanger_guids.txt

@rem ------------------------------------------------------------
@rem  downloading common scripts 
set URL=https://openrtm.org/svn/msi-buildtool/trunk/scripts
%SHELL%svn co --username %SVN_USER% --password %SVN_USER_PASSWD% %URL% scripts

@rem ------------------------------------------------------------
@rem  unzip binary files 
%SHELL%bash prepare_build.sh

@rem ------------------------------------------------------------
@rem  get GUID 
call scripts\getGuid.bat %PKG_NAME%%TOOL_VERSION:.=%_%ARCH%_Msm %GUIDS_FILE%
@set MSM_GUID=%GUID%

@rem ------------------------------------------------------------
@rem  make wxs file (*_files.wxs) 
@set PYTHONBIN=C:\python27\python.exe
%PYTHONBIN% scripts\wxsmaker.py -i -t changer_runtime/utils -p %PKG_NAME% -o %PKG_NAME%_files.wxs

@rem ------------------------------------------------------------
@rem  run candle.exe and light.exe 
@set MSM_FILE_NAME=%MSM_VER%_%ARCH%.msm
call scripts\build_wix_module.bat %MSM_FILE_NAME%

if not exist MergeModules mkdir MergeModules
@move /Y *.msm MergeModules
@del *.wixobj *.wixpdb
@del *_files.wxs *~

@set GUID=
@set MSM_FILE_NAME=

@echo on