# ParanoidFFD
Unblockable FanFiction Downloader

I was using FanFicFare to download my fanfics, but recently it seems that Fanfiction.net is implementing cloudflare which blocks automated downloader scripts.

So I made my own "unblockable" downloader using [undetected_chromedriver](https://github.com/ultrafunkamsterdam/undetected-chromedriver).

### Dependency
Latest stable build of Google Chrome / Chromium in the default install path (As of ParanoidFFD v1.1.0, you can use `-C <Chrome Version` to use other versions of Chrome)

Python 3 (3.6 or higher)

[undetected_chromedriver](https://pypi.org/project/undetected-chromedriver/)
### Build dependency
GCC and its' libraries or some other C compiler

Python3 dev package (with Python.h header)
## Usage
`ParanoidFFD <options>`
### Examples
`ParanoidFFD -u "my/fan/fiction/url"`

Download fanfiction from `my/fan/fiction/url` and save it to a text file (default format) with the filename as the fanfiction title (default filename).

`ParanoidFFD -f txt -o "my fanfiction save" -u "my/fan/fiction/url"`

Download fanfiction from `my/fan/fiction/url` and save it to a text file with the filename as `my fanfiction save.txt`.

`ParanoidFFD -C 86 --check-update`

Check for new updates for ParanoidFFD with Chrome 86 installed (Not latest stable build)

`ParanoidFFD --help`, `ParanoidFFD -h`

Show help page
## Installation (Linux)
```
git clone https://github.com/Paranoid-Dev/ParanoidFFD.git
cd ParanoidFFD
pikaur -S google-chrome-stable # for ArchLinux - use your pacakge manager for your OS
pacman -S python3 # for ArchLinux - use your pacakge manager for your OS
pip3 install undetected-chromedriver
gcc downloader-dynamic.c -o ParanoidFFD -I/usr/include/python3.9 -lpython3.9 # change python version and Python.h include path to match your system
```
#### How to find Python.h include path and Python version
```
$ sudo find / -name Python.h
/usr/include/python3.9/Python.h
/usr/include/python2.7/Python.h
```
So in my case, my Python version is 3.9, and the Python.h include path is /usr/include/python3.9
## Installation (Windows)
Installation for Windows is the same as for Linux - have Python 3.6 or higher installed, install undetected-chromedriver, and compile. (No compile scripts or pre-compiled binary yet, sorry - If you compiled a Windows version, please share your compile method and binary :) )

I have some thoughts about using wsl, since it's a painless way to run ParanoidFFD.
## Installation (Mac)
I *think* installation for Mac OS will be same as Linux (same gcc command - just change python include path and version), but since I don't have a Mac device, I can't say for sure.

Install GCC (see link below)

**(Below code are all untested)**

`sudo find / -iname "Python.h"` : Find Python.h include path and Python version

(change Python version and Python.h include path, and Python library path to match your system)

`gcc downloader-dynamic.c -o ParanoidFFD -I/Library/Frameworks/Python.framework/Versions/3.9/include/python3.9 -L/Library/Frameworks/Python.framework/Versions/3.9/lib -lpython3.9` 

See these links below for compilation help

https://stackoverflow.com/questions/59977457/installing-gcc-on-macos-catalina

https://github.com/Paranoid-Dev/ParanoidFFD/issues/1
## Discord
discord.gg/vuyt6HSSFb

Get the `Paranoid Programmer` role to join the (yet small) ParanoidFFD community