# ParanoidFFD
Undetectable FanFiction Downloader with Parallel Downloading

I was using FanFicFare to download my fanfics, but Fanfiction.net implemented cloudflare which blocks automated downloader scripts, leading to FanFicFare not working. 

So I made my own "undetectable" downloader, ParanoidFFD.

### Dependency
Chromium appimage and chromedriver for ParanoidFFD in the same directory as ParanoidFFD (Linux only) (recommended) _or_ chromedriver in the same directory as ParanoidFFD and install of Google Chrome / Chromium in the default install path

Python3 (3.6 or higher)

Python Selenium

Python Zlib module (Probably installed by default)

You can delete every file/folder in this repo once you've compiled ParanoidFFD - ParanoidFFD doesn't require any files/folders in this repo to run. You need only chromedriver (and appimage). 
### Build dependency
C compiler

Python3 dev package (with Python.h header)
## Usage
```
$ ./ParanoidFFD --help
                                                                                          
 ________________________________________________________________________________________
 Usage    : ParanoidFFD <options>                                                         
                                                                                          
 Options  :                                                                               
   -u <fanfiction url> : download from <fanfiction url>                                   
   -f <FORMAT>         : save fanfiction as <FORMAT>                                      
                         currently supported format : txt, epub                           
                         default : txt                                                    
   -o <FILE_NAME>      : save fanfiction as <FILE_NAME>.<FORMAT>                          
                         default : fanfiction_title.<FORMAT>                              
   -t <n>              : numder of parallel download threads, use a positive integer      
                         default : 3                                                      
   --version           : show ParanoidFFD version                                         
   --check-update      : check for new updates                                            
   -h , --help         : show this page                                                   
                                                                                          
 Examples :                                                                               
   ParanoidFFD -u "my/fan/fiction/url"                                                    
   ParanoidFFD -f epub -t 4 -o "my fanfiction save" -u "my/fan/fiction/url"               
   ParanoidFFD --check-update                                                             
 ________________________________________________________________________________________
 ```
## Installation (Linux)
```
git clone https://github.com/Paranoid-Dev/ParanoidFFD.git
cd ParanoidFFD
pip3 install selenium
gcc parallel-downloader.c -o ParanoidFFD -I/usr/include/python3.9 -lpython3.9 # change python version and Python.h include path to match your system
```
#### How to find Python.h include path and Python version
```
$ sudo find / -name Python.h
/usr/include/python3.9/Python.h
/usr/include/python2.7/Python.h
```
So in my case, my Python version is 3.9, and the Python.h include path is /usr/include/python3.9
## Installation (Windows)
Installation for Windows should be the same as for Linux. I'm working on building a single exe file, but for now, you'll have to compile it yourselves. 
## Installation (Mac)
### For "*brew* installed Python" users
```
brew install python@3.9
pip3 install selenium

# for apple silicon
clang parallel-downloader.c -o ParanoidFFD -I/opt/homebrew/Frameworks/Python.framework/Versions/3.9/include/python3.9 -L/opt/homebrew/Frameworks/Python.framework/Versions/3.9/lib -lpython3.9

# for intel
clang parallel-downloader.c -o ParanoidFFD -I/usr/local/homebrew/Frameworks/Python.framework/Versions/3.9/include/python3.9 -L/usr/local/homebrew/Frameworks/Python.framework/Versions/3.9/lib -lpython3.9
```

### For "*non-brew* installed Python" users
`sudo find / -iname "Python.h"` : Find Python.h include path and Python version

Change Python version and Python.h include path, and Python library path to match your system and apply to above.
## Discord
[discord.gg/vuyt6HSSFb](https://discord.gg/vuyt6HSSFb)
Get the `Paranoid Programmer` role to join the (yet small) ParanoidFFD community