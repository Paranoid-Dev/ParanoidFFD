# ParanoidFFD
Unblockable FanFiction Downloader

I was using FanFicFare to download my fanfics, but recently it seems that Fanfiction.net is implementing cloudflare which blocks automated downloader scripts.

So I made my own "unblockable" downloader using [undetected_chromedriver](https://github.com/ultrafunkamsterdam/undetected-chromedriver).

## Installation (Linux)
### Dynamic
```
git clone https://github.com/Paranoid-Dev/ParanoidFFD.git
cd ParanoidFFD
pikaur -S google-chrome-stable # for ArchLinux - use your pacakge manager for your OS
pacman -S python3 # for ArchLinux - use your pacakge manager for your OS
pip3 install undetected-chromedriver
gcc downloader-dynamic.c -o ParanoidFFD -I/usr/include/python3.9 -lpython3.9 # change python version and include path to match your system
```
**Note** Dynamic version requires the latest stable Google-Chrome / Chromium install in the default location. If your system fails to meet this, use `-C <Chrome Version>` to make ParanoidFFD use your version of Chrome.
### Portable
I decided that having over 300MB of portable Chrome just wasn't that acceptable so I'm dropping support for it.

Use `-C <Chrome Version>` to use your version of Chrome.
## Installation (Windows)
Installation for Windows is the same as for Linux - have Python 3.6 or higher installed, install undetected-chromedriver, and compile. (No compile scripts or pre-compiled binary yet, sorry - If you compiled a Windows version, please share your compile method and binary :) )

I have some thoughts about using wsl, since it's a painless way to run ParanoidFFD.
## Installation (Mac)
I *think* installation for Mac OS will be same as Linux (same gcc command - just change python include path and version), but since I don't have a Mac device, I can't say for sure.

https://github.com/Paranoid-Dev/ParanoidFFD/issues/1
## Usage
`ParanoidFFD <options>`
### Examples
`ParanoidFFD -u "my/fan/fiction/url"`
Download fanfiction from my/fan/fiction/url and saves it to a text file (default format) with the filename as the fanfiction title (default filename).
`ParanoidFFD -f txt -o "my fanfiction save" -u "my/fan/fiction/url"`
Download fanfiction from my/fan/fiction/url and saves it to a text file with the filename as `my fanfiction save.txt`.
`ParanoidFFD --help`,`ParanoidFFD -h`
Show help page