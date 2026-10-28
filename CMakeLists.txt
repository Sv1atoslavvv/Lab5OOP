cmake_minimum_required(VERSION 3.16)
project(profspilka_oop_lab5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(profspilka_oop_lab5
  src/main.cpp
  src/domain.cpp
  src/voucher.cpp
  src/services.cpp
  src/inheritance_demo.cpp
)
target_include_directories(profspilka_oop_lab5 PRIVATE include)
