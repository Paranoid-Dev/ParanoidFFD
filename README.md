# ParanoidFFD
Unblockable FanFiction.net Downloader

I was using FanFicFare to download my fanfics, but recently it seems that Fanfiction.net is implementing cloudflare which blocks automated downloader scripts.

So I made my own "unblockable" downloader using [undetected_chromedriver](https://github.com/ultrafunkamsterdam/undetected-chromedriver).

## Installation (Linux)
### Dynamic
```
pikaur -S google-chrome # just here to demonstrate you need the latest google-chrome / chromium
pacman -S python3
pip3 install undetected-chromedriver
./portable-compile.sh
```
**Note** Dynamic version requires the latest google-chrome / chromium install in the default location. If your system fails to meet this, use the portable version
### Portable
```
pacman -S python3
pip3 install undetected-chromedriver
mkdir vendor
cd vendor
wget "https://www.googleapis.com/download/storage/v1/b/chromium-browser-snapshots/o/Linux_x64%2F827102%2Fchrome-linux.zip?generation=1605233458736188&alt=media" -O chrome-linux.zip
python3 -m zipfile -e chrome-linux.zip ./
chmod +x chrome-linux/chrome
cd ..
./portable-compile.sh
```
## Installation (Windows)
I haven't tried to compile this on Windows yet, but the code will work on Windows too.

Just make sure Python 3.6 or higher is installed, and `pip install undetected-chromedriver`

Download static compiled chromium from [here](https://www.googleapis.com/download/storage/v1/b/chromium-browser-snapshots/o/Win_x64%2F827102%2Fchrome-win.zip?generation=1605234361586654&alt=media) and rename the folder name to `chrome-win`.
