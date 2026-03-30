## Solved in 2026

## Structure
- There is a single `main.cpp` which parses CLI arguments and then selects the right subprogram based on those arguments.
- CMake will build this into a single executable placed inside `build/`.
- Each day is a folder inside `src/` with a single header file, so you can easily import the solver functions for each day by just including that single header file.
- The inputs for each day are inside `inputs/`

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

## Usage of AI
Since I used this AoC as an opportunity to improve my C++ skills, I did allow myself to use AI for general C++ questions, for example about syntax. I did **NOT** provide it the puzzle text or any information about advent of code. I used it **ONLY** for questions like:
- *"How do I work with things like `vector.begin()` and `vector.end()` i.e. retrieve the value or move it forwards one step?"*
- *"How do I use `std::istringstream`?"*

**All** the ideas for how to solve the puzzles are **my own** and my own only.