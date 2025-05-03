## Build Environment Installation Guide

Dependencies required to build and run the project:
```shell
sudo apt update;
sudo apt install -y make clang clang-19 libglfw3 libglfw3-dev;
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 100;
```

Guide is for Ubuntu 24.04. For other linux distributions check with the package manager.

Will probably not work on MacOS or Windows.
