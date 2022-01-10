rm -rf ./build/gcc-debug
rm -rf ./build/gcc-release
rm -rf ./build/vs

mkdir ./build/gcc-debug
mkdir ./build/gcc-release
mkdir ./build/vs

cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-Og -g -Wall -Wpedantic -Wextra -Wformat -DDEBUG" -S . -B ./build/gcc-debug
cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w -O3 -DRELEASE" -S . -B ./build/gcc-release
cmake -G "Visual Studio 16 2019" -S . -B ./build/vs
