convert -depth 8 $1  rgb:logo.raw
#注意：rbg2565 在android/out/host/linux-x86/bin目录拷过来
rgb2565 -rle <logo.raw> initlogo.rle
