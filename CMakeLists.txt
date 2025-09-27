cmake_minimum_required(VERSION 3.10)

project(Camera_Project1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 如果编译器不支持 C++17，就报错并停止

find_package(OpenCV REQUIRED)   # 查找opencv库

add_subdirectory(lib)
add_subdirectory(app)