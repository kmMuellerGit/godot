#!/bin/bash
set -e
set -o pipefail

script_name=$(basename $0)
if "$(pgrep -fc "${script_name}" )" -gt 1 ; then
	echo "Script was already running."
	exit 1
fi

echo "Beginning background build.."
/usr/bin/time -v /bin/sh -c ' ./build.py --build binary    --mode production --tracy --disable_compilation_db --cores 9 ; date ' 2>&1 | tee ./build_editor_production.log
#/usr/bin/time -v /bin/sh -c ' ./build.py --build templates --mode debug      --tracy --disable_compilation_db --cores 3 ; date ' 2>&1 | tee ./build_template_debug.log
/usr/bin/time -v /bin/sh -c ' ./build.py --build templates --mode production --tracy --disable_compilation_db --cores 9 ; date ' 2>&1 | tee ./build_template_production.log
/usr/bin/time -v /bin/sh -c ' ./build.py --build binary    --mode release_production --disable_compilation_db --cores 9 ; date ' 2>&1 | tee ./build_editor_release_production.log

# strip debug
godots=$(find ./bin -maxdepth 1 -type f | grep "godot" | grep -v "debug")
for godot in $godots; do
	strip --strip-all "${godot}"
done
echo "Completed background build."
