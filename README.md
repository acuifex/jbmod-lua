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
1. Install `git` and run the command `git clone https://github.com/ValveSoftware/source-sdk-2013` then head to `<path-to-sourcesdk2013>\sp\src\utils\`
2. Clone the repo in this directory with this command `git clone https://github.com/acuifex/jbmod-lua luaplugin` there should be a new directory named `luaplugin`
3. now clone the [Lua repository](https://github.com/lua/lua) into the `luaplugin` directory with the command `git clone https://github.com/lua/lua.git`
4. then head back to `src` and download the `.lib` from [Lua's Sourceforge](https://sourceforge.net/projects/luabinaries/) Windows Libraries > Static and select the one that uses Win32 and move it to `lib\public`
5. now edit the `project.vgc` located in `sp|mp\src\vpc_scripts` now add this line in the top of the file (above `captioncompiler`)
```
$Project "Lua"
{
	"utils\luaplugin\luaplugin.vpc"
}
``` 
6. then edit `groups.vgc` and in the `game` group and `everything` group add
```
"luaplugin"
```
7. then run `creategameprojects.bat` or `createeverythingprojects.bat` and build the new `lua` project
8. move the `luaplugin.dll` to `steam/steamapps/common/JBMod/jbmod`
9. run jbmod with the `-insecure` flag
10. in jbmod run the command `plugin_load lua`
11. you should now be able to run `lua` commands such as `lua_runstring "print(42+69)"`

# TODO
- do the compiling at least somewhat properly. see https://github.com/YaLTeR/SourcePauseTool for copypasting
- game must be run with -insecure. figure out something
- ~compile for windows~ https://github.com/acuifex/jbmod-lua/pull/1
