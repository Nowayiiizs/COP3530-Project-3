cmake_minimum_required(VERSION 3.27)
project(Project3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Project3 main.cpp src/RBTree.h src/HashTable.h)
