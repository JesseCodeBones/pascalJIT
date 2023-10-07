# Pascal JIT Compiler

# Purpose
Most hacker do the hack with static code, like disassembly, debug and hook.
with static code, hacker almost can do anything with the application.  
So I created this simple JIT compiler, this JIT compiler directly generate machine code without dependencies like LLVM or GCC even GNU LD, this compiler also contains a very basic runtime with glibc or POSIX basic APIs.  
the target language of the compiler is Pascal.  
this compiler can work with other anti-disassembly tools like SSL check, assembly Obfuscation and etc.

### Build
`cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`
### Run Demo
`./build/pascal_jit_compiler ./tests/cases/helloworld.pas`
### Limitation
> Only support ARM 64 platform now