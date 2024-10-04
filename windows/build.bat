@echo off
pushd %~dp0

:: Use mingw to compile
:: TODO
gcc -shared -o luaplugin.dll ..\src\*.c -Wl,--out-implib,libluaplugin.a

popd
