#!/bin/sh
rmmod rgb
rm /dev/rgb
make
insmod rgb.ko
mknod /dev/rgb c 60 0
gcc rgb_test_util.c -o rgb_test_util
./rgb_test_util
