## How To Use
It should run on any \*nux system. It has been tested on:
- ArchLinux
- Ubuntu / Debian
- Mac OS X

#### for mac os x
- Install arm-none-eabi-gcc Cross Compile Toolchain
```
brew tap blanboom/homebrew-mcu-devtools
brew install gcc-arm-none-eabi
```

- Install stlink
```
brew install stlink
```
you can see the infomation by typing `brew info stlink`
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

- you can run `make bulid`

#### for other
You can do it by yourself!
