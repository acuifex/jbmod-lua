# Compiling for linux
https://developer.valvesoftware.com/wiki/Source_SDK_2013#Source_SDK_2013_on_Linux
1. clone source sdk
2. clone this repo into `sp/src/utils/`
3. download lua 5.4.4 or something similar into `sp/src/utils/luaplugin/` and rename to `lua`

    the directory tree should look like this: `sp/src/utils/luaplugin/lua/src/lua.hpp`
4. run `make MYCFLAGS="-m32" MYLDFLAGS="-m32" a` in `sp/src/utils/luaplugin/lua/src/`
5. copy or move `liblua.a` to `sp/src/lib/public/linux32/`
6. (optional) run `make clean` in `sp/src/utils/luaplugin/lua/src/`

7. apply sdk patches so it'll compile lol (or don't, i'm not your dad)

    `git apply --ignore-space-change "./sp/src/utils/luaplugin/sdk patches"`
8. `./create.sh`
9. `./build.sh`
10. plop the file from `sp/game/bin/luaplugin.so` into `steam/steamapps/common/JBMod/jbmod/lua.so`
11. set `-insecure` flag
12. `plugin_load lua`
13. `lua_runstring "print(42+69)"`


# Compiling for windows
Added with https://github.com/acuifex/jbmod-lua/pull/1. Apparently requires WSL (Windows Subsystem for Linux) and MinGW GCC.

Download and run `windows/compile.bat`. This will complete steps equivelent to 1-9 on linux.


# TODO
- do the compiling at least somewhat properly. see https://github.com/YaLTeR/SourcePauseTool for copypasting
- game must be run with -insecure. figure out something
- ~compile for windows~ https://github.com/acuifex/jbmod-lua/pull/1
