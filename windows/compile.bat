@echo off
mkdir %USERPROFILE%\source-sdk-2013
cd %USERPROFILE%\source-sdk-2013

wsl git clone https://github.com/ValveSoftware/source-sdk-2013.git
wsl git clone https://github.com/acuifex/jbmod-lua.git source-sdk-2013/sp/src/utils/luaplugin

wsl mkdir -p ~/source-sdk-2013/sp/src/utils/luaplugin/lua
wsl cd ~/source-sdk-2013/sp/src/utils/luaplugin/lua && wget http://www.lua.org/ftp/lua-5.4.4.tar.gz
wsl tar -xzf ~/source-sdk-2013/sp/src/utils/luaplugin/lua/lua-5.4.4.tar.gz -C ~/source-sdk-2013/sp/src/utils/luaplugin/lua
wsl mv ~/source-sdk-2013/sp/src/utils/luaplugin/lua/lua-5.4.4 ~/source-sdk-2013/sp/src/utils/luaplugin/lua/src

wsl cd ~/source-sdk-2013/sp/src/utils/luaplugin/lua/src && make MYCFLAGS="-m32" MYLDFLAGS="-m32" a

wsl mv ~/source-sdk-2013/sp/src/utils/luaplugin/lua/src/liblua.a ~/source-sdk-2013/sp/src/lib/public/linux32/

wsl cd ~/source-sdk-2013/sp/src/utils/luaplugin && git apply --ignore-space-change ./sdk\ patches

call create.bat
call build.bat

pause
