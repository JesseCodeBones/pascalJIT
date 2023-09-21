## gtest build
1. clone googletest
2. `cmake -B build -DCMAKE_CROSSCOMPILING_EMULATOR=qemu-aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ && cmake --build build --parallel`

## build compiler
1. gtest package
  `export GTest_ROOT=/home/jesse/workspace/aarch64_builds/googletest/build` 
2. build  
`cmake -B build -DCMAKE_CROSSCOMPILING_EMULATOR=qemu-aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ && LD_LIBRARY_PATH=/usr/aarch64-linux-gnu/lib/ cmake --build build --parallel`
3. test  
`LD_LIBRARY_PATH=/usr/aarch64-linux-gnu/lib/ ./pascal_jit_e2e_test`
4. 编译执行快捷方式：
`LD_LIBRARY_PATH=/usr/aarch64-linux-gnu/lib/ make && LD_LIBRARY_PATH=/usr/aarch64-linux-gnu/lib/ ./pascal_jit_e2e_test`
