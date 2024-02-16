#!/usr/bin/bash
set -e


scons --clean 2>&1 | tee clean.sh.log
# find . -name \*.gen.* -type f -delete 2>&1 | tee -a clean.sh.log
# find . -name \*.lib -type f -delete 2>&1 | tee -a clean.sh.log
# find . -name \*.obj -type f -delete 2>&1 | tee -a clean.sh.log
# find . -name \*.lnk -type f -delete 2>&1 | tee -a clean.sh.log

custom_modules_path="../cyborg_survivors_game/engine_custom_modules"
for path in . ${custom_modules_path}; do
    
    find ${path} -name "*.gen.*" -type f -exec rm -v {} \; 2>&1 | tee -a clean.sh.log
    find ${path} -name "*.lib" -type f -exec rm -v {} \; 2>&1 | tee -a clean.sh.log
    find ${path} -name "*.obj" -type f -exec rm -v {} \; 2>&1 | tee -a clean.sh.log
    find ${path} -name "*.o" -type f -exec rm -v {} \; 2>&1 | tee -a clean.sh.log
    find ${path} -name "*.a" -type f -exec rm -v {} \; 2>&1 | tee -a clean.sh.log
    find ${path} -name "*.lnk" -type f -exec rm -v {} \; 2>&1 | tee -a clean.sh.log
    find ${path} -name "__pycache__" -type d -exec rm -rfv {} \; 2>&1 | tee -a clean.sh.log
done
echo "Complete"

