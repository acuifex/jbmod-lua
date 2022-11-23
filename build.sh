#!/bin/bash

pushd `dirname $0`
make -f ../../luaplugin.mak
popd

