## Setting up CMake
In the source directory:
```
cmake -S . -B build
```

This tells CMake that the source directory is the current folder (`-S .`) and that the build files should be written to build (`-B build`)
This should be run each time `CMakeLists.txt` is modified

Alternative: Building release:
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```


Then to build the main binary:
```
cmake --build build
```


And to run Day 1, Part 2 with the example input:
```
./build/aoc 1 2 --example
```