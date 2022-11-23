https://developer.valvesoftware.com/wiki/Source_SDK_2013#Source_SDK_2013_on_Linux

1. clone source sdk
1. clone this repo into `sp/src/utils/`
1. download lua 5.4.4 or something similar into `sp/src/utils/luaplugin/` and rename to `lua`

    the directory tree should look like this: `sp/src/utils/luaplugin/lua/src/lua.hpp`
1. run `make MYCFLAGS="-m32" MYLDFLAGS="-m32" a` in `sp/src/utils/luaplugin/lua/src/`
1. copy or move `liblua.a` to `sp/src/lib/public/linux32/`
1. (optional) run `make clean` in `sp/src/utils/luaplugin/lua/src/`

1. apply sdk patches so it'll compile lol (or don't, i'm not your dad)

    `git apply --ignore-space-change "./sp/src/utils/luaplugin/sdk patches"`
1. `./create.sh`
1. `./build.sh`
1. plop the file from `sp/game/bin/luaplugin.so` into `steam/steamapps/common/JBMod/jbmod/lua.so`
1. set `-insecure` flag
1. `plugin_load lua`
1. `lua_runstring "print(42+69)"`

TODO:
- do the compiling at least somewhat properly. see https://github.com/YaLTeR/SourcePauseTool for copypasting
- game must be run with -insecure. figure out something
- compile for windows
