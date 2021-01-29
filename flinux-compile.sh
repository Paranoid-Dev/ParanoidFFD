#!/bin/bash
gcc $(python-config --cflags) $(python-config --ldflags) -static downloader-flinux.c -o ParanoidFFD
