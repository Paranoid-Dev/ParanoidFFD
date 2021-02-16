# ParanoidFFD
Unblockable FanFiction Downloader

I was using FanFicFare to download my fanfics, but recently it seems that Fanfiction.net is implementing cloudflare which blocks automated downloader scripts.

So I made my own "unblockable" downloader using [undetected_chromedriver](https://github.com/ultrafunkamsterdam/undetected-chromedriver).

## Installation (Linux)
### Dynamic
```
pikaur -S google-chrome-stable # just here to demonstrate you need the latest stable google-chrome / chromium
pacman -S python3 # install Python3
pip3 install undetected-chromedriver
gcc downloader-dynamic.c -o ParanoidFFD -I/usr/include/python3.9 -lpython3.9 # change python version to match your system
```
**Note** Dynamic version requires the latest google-chrome / chromium install in the default location. If your system fails to meet this, set options to make ParanoidFFD use your version of Chrome.
### Portable
I decided that having over 300MB of portable chrome just wasn't that acceptable so I'm dropping support for it.

I'll put in an option where you can select which version of google-chrome/chromium you have installed if you don't have the latest stable build.
## Installation (Windows)
Installation for Windows is the same as for Linux - have Python 3.6 or higher installed, install undetected-chromedriver, and compile. (No compile scripts or pre-compiled binary yet, sorry - If you compiled a Windows version, please share your compile method and binary :) )

I have some thoughts about using wsl, since it's a painless way to run ParanoidFFD.
## Installation (Mac)
I *think* installation for Mac OS will be same as Linux (same gcc command - just change python include path and version), but since I don't have a Mac device, I can't say for sure.

https://github.com/Paranoid-Dev/ParanoidFFD/issues/1