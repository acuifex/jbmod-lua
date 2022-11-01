https://developer.valvesoftware.com/wiki/Source_SDK_2013#Source_SDK_2013_on_Linux

1. clone this repo
1. clone source sdk or something
1. clone lua 5.4.4 or something similar
1. copy .cpp and .vpc files into `sp/src/utils/serverplugin_sample/`
1. copy src from lua-5.4.4 into `sp/src/utils/serverplugin_sample/` and rename to lua
1. add `-m32` to makefile in lua, so it'll compile 32 bit
	```
	CFLAGS= -m32 -O2 -Wall -Wextra -DLUA_COMPAT_5_3 $(SYSCFLAGS) $(MYCFLAGS)
	LDFLAGS= -m32 $(SYSLDFLAGS) $(MYLDFLAGS)
	```
1. run `make` for lua
1. move `liblua.a` to `sp/src/lib/public/linux32/`
1. apply sdk patches so it'll compile lol (or don't, i'm not your dad)
1. `./createallprojects`
1. `make -f ./everything.mak serverplugin_empty`

1. plop the file from `sp/game/bin/serverplugin_empty.so` into `steam/steamapps/common/JBMod/jbmod/lua.so`
1. set `-insecure` flag
1. `plugin_load lua`
1. `lua_runstring "print(42+69)"`

TODO:
- createallprojects and everything.mak are probably unnecessary.
- do the compiling at least somewhat properly. see https://github.com/YaLTeR/SourcePauseTool for copypasting
- game must be run with -insecure. figure out something
- compile for windows