@Rem Batch for gvsbuild launch
@echo off
set _BASE_DIR=%~dp0
pushd %_BASE_DIR%
%_BASE_DIR%py\python.exe %_BASE_DIR%build.py %*
popd
set _BASE_DIR=
