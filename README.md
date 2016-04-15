## How To Use
It should run on any \*nux system. It has been tested on:
- ArchLinux
- Ubuntu / Debian
- Mac OS X

#### for mac os x
1. Install arm-none-eabi-gcc Cross Compile Toolchain
```
brew tap blanboom/homebrew-mcu-devtools
brew install gcc-arm-none-eabi
```

2. Install stlink
```
brew install stlink
```
you can the infomation by use `brew info stlink`
```
stlink: stable 1.1.0 (bottled), HEAD
stm32 discovery line Linux programmer
https://github.com/texane/stlink
/usr/local/Cellar/stlink/1.1.0 (12 files, 255.7K) *
  Poured from bottle
From: https://github.com/Homebrew/homebrew-core/blob/master/Formula/stlink.rb
==> Dependencies
Build: autoconf ✔, automake ✔, pkg-config ✔
Required: libusb ✔
```

3. you can run `make bulid`
