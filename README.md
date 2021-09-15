# Visu
3D data viewer, written in C with OpenGL

Based on [pBase](https://github.com/renehorstmann/pbase)

## Getting started

### Install
see pbase install section.

### Usage
To use pbase in your cmake project:
```cmake
cmake_minimum_required(VERSION 3.0)
project(visutest C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c)

if (MINGW) # windows
  include_directories("C:/Libraries/include")
endif()

find_library(PBASE_LIB pbase)
find_library(VISU_LIB visu)
target_link_libraries(main ${PBASE_LIB} ${VISU_LIB})
```

See [svisu_exmaple.c](tests/svisu_example.c) for a code example

## Author

René Horstmann

## Licence

This project is licensed under the MIT License - see the LICENSE file for details

