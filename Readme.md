C+ code for deployment on Android, Windows, linux, MacOS

# Environment setup

Build requirements:
- Git (for clone codes and submodules)
```
git clone --recurse-submodules git@git.woa.com:InteractionAlgorithm/Juglans.git
```
- CMake 3.21.0 or later
- C++ 17 compiler
  - MSVC 2017 version 15.7 or later, MSVC 2022 is recommended.
  - GCC 7.0 or later
  - Clang 6.0 or later
  - Android NDK r17, see instructions below
- (Optional) If build with command line, install `make`, `jom` or `ninja`. Recommend to use `ninja` on Windows and `make` on other platforms.
- Have clang-format 14 in PATH, see [Code format section](#code-format) for details.
- TensorRT (Windows and Linux), see instructions below

## Android Setup

These steps assumes you haven't installed Android SDK or NDK before.
If you already has Android SDK, you can install NDK r17c and set it folder as `ANDROID_NDK_r17c` environment variable.

It's recommended to use NDK r17c to match the same version that is used by SNPE.

1. Download [Tencent JDK 8](http://jdk.oa.com/download.html).
  1. Extract binary and set `TencentKona-8.xxx` folder as `JAVA_HOME` environment variable
  2. Add `TencentKona-8.xxx/bin` folder to `PATH`
2. Download [Android command line tools](https://developer.android.com/studio#command-tools).
  1. Extract binary to a temp place
  2. Create a new folder for android sdk on the disk. Referred as `<Android SDK Path>` in following commands.
  3. Run `cmdline-tools\bin\sdkmanager.bat --sdk_root=<Android SDK Path> "cmdline-tools;latest" platform-tools "ndk;21.4.7075529"`
  4. Add `<Android SDK Path>\platform-tools` and optionally `<Android SDK Path>\cmdline-tools\latest\bin` to `PATH`.
  5. Set `<Android SDK Path>\ndk\21.4.7075529` as `ANDROID_NDK_r21e` environment variable.
  6. Remove the temp command line tools

## Install TensorRT

1. Install latest NVIDIA GPU driver from [official website](https://www.nvidia.com/Download/index.aspx?lang=en-us)
2. Install [CUDA Toolkit 11.3 Update 1](https://developer.nvidia.com/cuda-11-3-1-download-archive), which is same version used by Pytorch in Hawthorn.
  - On Windows, download and run setup exe. Make sure unselect GPU driver as we have installed newer version in previous step.
  - On Linux (including WSL2), follow instruction on pages to install with package manager.
3. [Register NVIDIA developer account](https://developer.nvidia.com/login), which is required to download cuDNN and TensorRT.
4. Download and install [latest cuDNN for CUDA 11.x](https://developer.nvidia.com/rdp/cudnn-download)
  - On Windows, extract zip file and add bin subfolder to PATH.
  - On Windows, extract `zlibwapi.dll` from [zlib release](http://www.winimage.com/zLibDll/zlib123dllx64.zip) and put in cuDNN bin folder (or any other foler in PATH).
5. Download and install [lastest TensorRT GA for CUDA 11.x](https://developer.nvidia.com/nvidia-tensorrt-8x-download).
  - On Windows, extract zip file and add lib & bin subfolders to PATH.

# Build Instructions

## Build on Windows using VS

Run the following commands in command line to generate VS solution and projects,
assuming current workdir is root of juglans repo.

```
md build
cd build
cmake -G "Visual Studio 17 2022" ..
start Juglans.sln
```

If you are using older VS version, change cmake generator to `Visual Studio 15 2017 Win64` or `Visual Studio 16 2019`.

## Build on Windows using Commane Line

1. Open Developer Command Prompt for VS
2. Run following commands to generate ninja makefiles, assuming current workdir is root of juglans repo.
```
md build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
```
3. To build targets, run `ninja <target>`.

## Build for Android

1. Make sure `ANDROID_NDK_r17c` environment variable is set.
2. Run following commands to generate ninja makefiles, assuming current workdir is root of juglans repo.
```
md build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DANDROID=ON ..
```
3. To build targets, run `ninja <target>`.

## Available targets

1. Module targets: `juglans_<module>`, e.g. `juglans_core`.
2. Tool targets: `<tool>`, e.g. `capture`.
3. Test targets: `<module>_test`, e.g. `core_test`.
4. Run test targets (command line generator only): `run_<module>_test` or `run_all` to run all tests.
5. Unity related targets: `unity` to copy binaries to unity demo's plugin folder.
6. Code format targets: `clangformat` to format all, or `clangformat_<module>`/`clangformat_<tool>` to format only 1 module/tool.

# Code format

We use clang-foramt 14 to format all source files in the repo.

## Install

* Windows
  1. Download `LLVM-14.0.5-win64.exe` from [release page](https://github.com/llvm/llvm-project/releases/tag/llvmorg-14.0.5)
  2. Install LLVM and add bin folder to PATH.
  3. Alternatively, copy `clang-format.exe` from bin folder and put it in any folder inside PATH.
* Mac
```
brew install clang-format@14
```
* Ubuntu
  * Ubuntu 18.04
  ```
  echo "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-14 main" | sudo tee /etc/apt/sources.list.d/llvm.list
  echo "deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic-14 main" | sudo tee -a /etc/apt/sources.list.d/llvm.list
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
  sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
  sudo apt install clang-format-14
  ```
  * Ubuntu 20.04 or newer
  ```
  echo "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-14 main" | sudo tee /etc/apt/sources.list.d/llvm.list
  echo "deb-src http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-14 main" | sudo tee -a /etc/apt/sources.list.d/llvm.list
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
  sudo apt update
  sudo apt install clang-format-14
  ```

## Format

Build `clangformat` target generated by CMake.
This can be `clangformat` project in your VS solution or `make clangforamt` depending on your CMake generator.
