cmake_minimum_required(VERSION 3.10)
project(mips)

set(CMAKE_CXX_STANDARD 11)

include_directories(./header ./src)

set(Sources main.cpp ./src/utility/memory.cpp ./src/utility/common.cpp
        ./src/utility/utility.cpp ./src/parse/parser.cpp ./src/parse/scanner.cpp
        ./src/operators/base.cpp ./src/operators/specialists.cpp ./src/simulator/coordinator.cpp
        ./src/simulator/processor.cpp src/simulator/simulator.cpp)

set(Headers header/utility/common.h header/utility/memory.h
        header/parse/parser.h header/parse/scanner.h header/simulator/coordinator.h
        header/simulator/processor.h header/operators/base.h header/operators/specialists.h
        header/utility/utility.h header/simulator/simulator.h)
add_executable(mips ${Sources} ${Headers})