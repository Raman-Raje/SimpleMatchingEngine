cmake_minimum_required(VERSION 3.10)
project(MatchingEngine)

# C++17 is just an example; adjust as needed
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally set where the final executables/libraries should be placed:
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add the executable by listing source files from 'src'
add_executable(MatchingEngine
    src/main.cpp
    src/amount.cpp
    src/order.cpp
    src/avlTree.cpp
    src/MatchingEngine.cpp

)