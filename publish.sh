#!/bin/bash

pushd `dirname $0`
mkdir publish
cp ../../../game/bin/luaplugin.so ./publish/lua.so
CONTENTFOLDER="$PWD/publish"
CHANGENOTE=`git rev-parse --short HEAD` # let's hope i don't forget to commit before publishing
export CONTENTFOLDER CHANGENOTE
cat publish.template.vdf | envsubst > publish.vdf
steamcmd +login $STEAMUSER +workshop_build_item "$PWD/publish.vdf" +quit
popd

