@echo off
pushd %~dp0

findstr /C:"$Project ""luaplugin""" ..\..\vpc_scripts\projects.vgc >nul
if errorlevel 1 (
    (
        echo $Project "luaplugin"
        echo {
        echo     "utils\luaplugin\luaplugin.vpc" [$WIN32||$POSIX]
        echo }
    ) >> ..\..\vpc_scripts\projects.vgc
)

..\..\devtools\bin\vpc @luaplugin /mksln luaplugin

popd
