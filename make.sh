#
reset
clear
# 
export website="/Users/kevin.jiang/Dev/src/github-res/fm.smk/website/apps/"
# 
emcmake cmake -B ./build -S . 
cd build
make