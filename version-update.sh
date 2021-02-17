#!/bin/bash
echo updating from version $1 to version $2 ...
sed -i -e "s/$1/$2/g" downloader-dynamic.c

cd "updates history"
echo "https://raw.githubusercontent.com/Paranoid-Dev/ParanoidFFD/main/updates%20history/$2" > "$2.tmp"
for f in *-n
do
  cp "$2.tmp" "$f"
done
rm "$2.tmp"
echo NA > "$2-n"
cp "$1" "$2"
sed -i -e "s|https://github.com/Paranoid-Dev/ParanoidFFD||g" "$1"
nano $1
nano $2