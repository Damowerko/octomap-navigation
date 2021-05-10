# octomap-navigation
C++ library that implements A* pathfinding directly on octomaps.

## Setup
Clone the repo and initialize submodules, either use 
```
git clone --recurse-submodules ...
```
or if you cloned the repo already 
```
git submodule init
git submodule update
```

The dependencies are all handled by `vcpkg` which integrates with CMAKE. To build the project make sure you have `cmake` and a compiler. Then you can configure and build the cmake project as follows.
```
mkdir build && cd build
cmake ..
cmake --build .
```