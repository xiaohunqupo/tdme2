![LOGO](https://raw.githubusercontent.com/andreasdr/a-gui/main/resources/github/agui-logo.png)

# 1. How to build A-GUI?

This document is WIP now, as we have switched the build system to use MinitScript scripts.

## 1.1. FreeBSD(tested on FreeBSD 13.0)

### 1.1.1 Install packages

```bash
su
pkg install git
pkg install gmake
pkg install openal-soft
pkg install glfw
pkg install glew
pkg install freetype2
exit
```

### 1.1.2. Clone repository

```bash
git clone https://github.com/andreasdr/a-gui
cd a-gui
```

### 1.1.3. Build

Note:
- HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4
- About YES|NO, just choose to type YES or NO, this enables the renderer plugin to be build additionally to GL3+/CORE renderer plugin.

```bash
gmake clean && gmake -j HARDWARE_THREADS mains VULKAN=YES|NO GLES2=YES|NO
```

## 1.2. OpenBSD(tested on OpenBSD 6.8)

### 1.2.1 Install packages

```bash
su
pkg_add git
pkg_add gmake
pkg_add glew
pkg_add openal
pkg_add glfw
pkg_add freetype2
exit
```

### 1.2.2. Clone repository

```bash
git clone https://github.com/andreasdr/a-gui
cd a-gui
```

### 1.2.3. Build

Note: HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores, you can use 2.

```bash
gmake clean && gmake -j HARDWARE_THREADS mains CXX=clang++ CC=clang
```

## 1.3. Ubuntu(tested on Ubuntu 20.04)

### 1.3.1 Install packages

```bash
sudo apt-get install git
sudo apt-get install make
sudo apt-get install g++
sudo apt-get install libopenal-dev
sudo apt-get install libglfw3-dev
sudo apt-get install libglew-dev
sudo apt-get install libfreetype-dev
```

### 1.3.2. Clone repository

```bash
git clone https://github.com/andreasdr/a-gui
cd a-gui
```

### 1.3.3. Build

Note:
- HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4
- About YES|NO, just choose to type YES or NO, this enables the renderer plugin to be build additionally to GL3+/CORE renderer plugin.

```bash
make clean && make -j HARDWARE_THREADS mains VULKAN=YES|NO GLES2=YES|NO
```

## 1.4. Mac OS X

### 1.4.1 Install packages

You have to install XCode or XCode command line tools.

### 1.4.1.1 Metal via Vulkan

For Metal via Vulkan you need to install Vulkan SDK for MacOSX using the installer script.

### 1.4.2. Clone repository

```bash
git clone https://github.com/andreasdr/a-gui
cd a-gui
```

### 1.4.3. Build

Note:
- HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4
- About YES|NO, just choose to type YES or NO, this enables the renderer plugin to be build additionally to GL3+/CORE renderer plugin.
- VULKAN=YES builds Vulkan renderer plugin, which uses Metal via Vulkan

```bash
make clean && make -j HARDWARE_THREADS mains VULKAN=YES|NO
```

## 1.5. Windows MSYS2/MINGW64(tested on Windows 10 Pro)

### 1.5.1 Install packages

Install 64 Bit MSYS2 from https://www.msys2.org/, e.g. msys2-x86_64-20190524.exe, then update your MSYS2 installation and install packages.

```bash 
pacman -Syu
pacman -S git
pacman -S make
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-openal
pacman -S mingw-w64-x86_64-glfw
pacman -S mingw-w64-x86_64-glew
pacman -S mingw-w64-x86_64-dlfcn
pacman -S mingw-w64-x86_64-freetype
```

### 1.5.2. Clone repository

```bash
git clone https://github.com/andreasdr/a-gui
cd a-gui
```

### 1.5.3. Build

Note:
- HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4
- About YES|NO, just choose to type YES or NO, this enables the renderer plugin to be build additionally to GL3+/CORE renderer plugin.

```bash
make clean && make -j HARDWARE_THREADS mains VULKAN=YES|NO
```

## 1.6. Windows MSC/X64(tested on Windows 10 Pro)

### 1.6.1 Install packages

Install Visual Studio 2022 Community Edition for C++ development, including MSVC v143 and Windows 10 SDK(10.0.19041.0).
Be aware that the makefile uses the default installation paths for libraries and headers.

### 1.6.2. Download or clone repository

Use a GIT client with the following URL: https://github.com/andreasdr/a-gui and clone to a folder into your home folder like "a-gui" OR 
send your browser to "https://github.com/andreasdr/a-gui" and click "Clone or download/Download ZIP" and extract ZIP somewhere in your home folder, e.g. "a-gui".

### 1.6.3. Build

Open your Visual Studio 2022 Community command line interface app for native X64 builds and execute the following commands. 
You need to adjust "a-gui" if you used a different folder. Also you need to extract ext/windows-msc/tools.zip to ext/windows-msc using 7zip or manually.

```
cd %HOMEPATH%/a-gui
cd ./ext/windows-msc && 7z x tools.zip && cd ../..
nmake -f Makefile.nmake
```

This will build A-GUI executables into your "a-gui" folder. You need to copy DLLs still to this folder, just that those dependencies are found.
Also this will build Vulkan renderer plugin as well as dependencies are included in A-GUI repository and its quite cheap to do so.
You need to adjust "a-gui" if you used a different folder.

```
cd %HOMEPATH%\a-gui
.\scripts\windows-prepare-platform-libs.bat
copy .\platform\libs\* .
copy .\lib\* .
```

### 1.6.4. Note

- All *.EXE files are generated in your "a-gui" folder root. You should just be able to launch them from there. So you can ignore 2.1, 2.2 and 2.3.
- The MSC build process is WIP

# 2. Running a A-GUI binary

You find binaries in
- ./bin/tdme/tools/cli

## 2.1. Unix-like platforms

On Unix like platforms you need to set up LD_LIBRARY_PATH, just that A-GUI libraries and renderer plugins are found.

```bash
$ pwd
/usr/home/andreas/Development/drewke.net/a-gui
$ export LD_LIBRARY_PATH=`pwd`/lib
```

## 2.2. MacOSX

On MacOSX you need to set up DYLD_LIBRARY_PATH, just that A-GUI libraries and renderer plugins are found.

```bash
$ pwd
/usr/home/andreas/Development/drewke.net/a-gui
$ export DYLD_LIBRARY_PATH=`pwd`/lib
```

## 2.3. MSYS2/MINGW64

On MSYS2/MINGW64 you need to copy shared objects files to A-GUI folder, just that A-GUI libraries and renderer plugins are found.

```bash
$ pwd
/usr/home/andreas/Development/drewke.net/a-gui
$ cp ./lib/*.dll .
```

## 2.4. Execute A-GUI binaries

You have to execute the binaries from a-gui root folder like:

```bash
$ pwd
/usr/home/andreas/Development/drewke.net/a-gui
$ ./bin/tdme/tools/cli/AGUITest
```

# 3. Other information

## 3.1. Links

- A-GUI, see [README.md](./README.md)
- A-GUI - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/tMqrmcp23Q)

## 3.2. Additional notes

- Build instructions will follow for NetBSD and Haiku if they get more recent graphics card support