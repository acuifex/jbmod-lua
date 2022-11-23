#!/bin/bash

pushd `dirname $0`

if ! grep -qxF '$Project "luaplugin"' ../../vpc_scripts/projects.vgc; then
	# append our project name, so vpc sees it
	cat >> ../../vpc_scripts/projects.vgc <<- EOF

\$Project "luaplugin"
{
	"utils\luaplugin\luaplugin.vpc" [\$WIN32||\$POSIX]
}
	EOF
fi
../../devtools/bin/vpc @luaplugin /mksln luaplugin
popd

