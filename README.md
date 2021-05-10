# octomap-navigation
C++ library that implements A* pathfinding directly on octomaps.

## Setup
After you clone the repo make sure you have initialized the submodules using `git submodule init` and `git submodule update`. The `vcpkg` submodule is an integrated package manager. To bootstrap vcpkg run `./vcpkg/bootstrap-vcpkg.sh` and to install the dependencies `./vcpkg/vcpkg install`.