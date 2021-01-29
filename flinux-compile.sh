#!/bin/bash
mkdir vendor
cd vendor
wget "https://www.googleapis.com/download/storage/v1/b/chromium-browser-snapshots/o/Linux%2F382086%2Fchrome-linux.zip?generation=1458337367735000&alt=media" -O chrome-linux.zip
python3 -m zipfile -e chrome-linux.zip ./
rm chrome-linux.zip
chmod +x chrome-linux/chrome
cd ..
gcc $(python3-config --cflags) -o ParanoidFFD $(python3-config --ldflags) -static downloader-flinux.c 
