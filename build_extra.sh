#!/bin/sh
set -e

echo "Beginning background build.."
/usr/bin/time -v ./build.py --build binary    --mode production --tracy --disable_compilation_db --cores 2 2>&1 | tee ./build_editor_production.log
/usr/bin/time -v ./build.py --build templates --mode debug      --tracy --disable_compilation_db --cores 2 2>&1 | tee ./build_template_debug.log
/usr/bin/time -v ./build.py --build templates --mode production --tracy --disable_compilation_db --cores 2 2>&1 | tee ./build_template_production.log
echo "Completed background build."
