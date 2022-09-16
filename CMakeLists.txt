# our cmake version
cmake_minimum_required(VERSION 3.5)

# project settings
project(codanim VERSION 0.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_subdirectory(mazesolver)
add_subdirectory(hmorphdyn) # holomorphic dynamics
