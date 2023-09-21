#include "../src/Assembly.hpp"
#include "../src/JIT.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime.hpp"
#include "../src/Token.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <gtest/gtest.h>
#include <vector>

const std::filesystem::path workspace =
    std::filesystem::current_path().parent_path().parent_path();

// TEST(compiler_e2e, base) {

//     std::ifstream t(workspace / "tests" / "cases" / "helloworld.pas");
//     std::stringstream buffer;
//     buffer << t.rdbuf();
//     buffer << EOF;

//     Tokenizer tokenizer(buffer.str());
//     Parser parser(tokenizer, std::make_shared<Runtime>());
//     auto program = parser.parse();

//     auto fun = createJit(program->codegen());
//     fun();
//  }
template< typename T>
void printAssemblyMachineCode(T&& runtime) {
  int index = 0;
  for(uint8_t code : runtime) {
    std::cout << std::hex << static_cast<uint32_t>(code);
    index++;
    if (index >= 4) {
      index = 0;
      std::cout << std::endl;
    } else {
      std::cout<< "_";
    }
  }
}
#define asm_debug
TEST(compiler_e2e, parser) {
  std::ifstream t(workspace / "tests" / "cases" / "printVariable.pas");
  std::stringstream buffer;
  buffer << t.rdbuf();
  Tokenizer tokenizer(buffer.str());
  Parser parser(tokenizer, std::make_shared<Runtime>());
  auto program = parser.parse();
  auto fun = createJit(program->codegen());
  std::cout << "ptr: 0x" << std::hex<< (void*) fun << std::endl;
  fun();
  // printAssemblyMachineCode(program->codegen());
}

void mylog(char* c){
  printf("%s\n", c);
}

void mylogi(int c) {
  printf("%d\n", c);
}

TEST(compiler_e2e, local_variable) {
  std::vector<uint8_t> executable;
  const char* test = "you are a good boy";
  const char* test2 = "I am jesse";
  addAssemblyToExecutable(executable, storeX29X30());
  addAssemblyToExecutable(executable, mov_register_register_from_to_sp(29, 31));
  addAssemblyToExecutable(executable, sub_register_imm(31, 31, 0x10));
  addAssemblyToExecutable(executable, insertPtrToRegister(9,test));
  addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x8));
  addAssemblyToExecutable(executable, str_register_register_offset(9, 10, 0));
  addAssemblyToExecutable(executable, insertPtrToRegister(9,test2));
  addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x10));
  addAssemblyToExecutable(executable, str_register_register_offset(9, 10, 0));
  addAssemblyToExecutable(executable, sub_register_imm(31, 31, 0x10));
  addAssemblyToExecutable(executable, insertIntegerToRegister(9,42));
  addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x18));
  addAssemblyToExecutable(executable, str_register_register_offset(9, 10, 0));
  addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x8));
  addAssemblyToExecutable(executable, ldr_register_register_offset(9, 10, 0));
  addAssemblyToExecutable(executable, mov_register_register(0, 9));
  addAssemblyToExecutable(executable, insertPtrToRegister(9, (void*)mylog));
  addAssemblyToExecutable(executable, callRegister(9));
  addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x18));
  addAssemblyToExecutable(executable, ldr_register_register_offset(9, 10, 0));
  addAssemblyToExecutable(executable, mov_register_register(0, 9));
  addAssemblyToExecutable(executable, insertPtrToRegister(9,(void*) mylogi));
  addAssemblyToExecutable(executable, callRegister(9));
  addAssemblyToExecutable(executable, mov_register_register_from_to_sp(31, 29));
  addAssemblyToExecutable(executable, loadX29X30());
  addAssemblyToExecutable(executable, ret());
  // asm("mov x0, #0xffffffff");
  // addAssemblyToExecutable(executable, insertIntegerToRegister(0, 0xffffffff));
  // printAssemblyMachineCode(executable);
  auto fun = createJit(executable);
  // std::cout << "ptr: 0x" << std::hex<< (void*) fun << std::endl;
  // fun();
}

//  TEST(compiler_e2e, variable) {
//     std::vector<uint8_t> result;
//     char* testString = "hello JIT\n";
//     addAssemblyToExecutable(result, storeX29X30());
//     addAssemblyToExecutable(result, insertPtrToRegister(0, testString));
//     Runtime runtime;
//     void* writter = runtime.nativeFunction["write"];
//     addAssemblyToExecutable(result, insertPtrToRegister(9, writter));
//     addAssemblyToExecutable(result, callRegister(9));
//     addAssemblyToExecutable(result, loadX29X30());
//     addAssemblyToExecutable(result, ret());
//     auto fun = createJit(result);
//     fun();

//     // stp x29, x30, [sp, -0x10]!

//     // sub sp, sp, 0x10
//     // str x0, [sp, #8] // 保存参数
//     // str x1, [sp]

//     // sub sp, sp, 0x10
//     // str x9, [sp, #8] //保存临时对象
//     // ldr x10, [sp, #8] //使用临时对象

//     // ldr x1, [sp]
//     // ldr x0, [sp, #8] // 使用参数

//     // 计算临时变量和参数综合，加到sp上
//     // add sp, sp, #0x20

//     // ldp x29, x30, sp, #0x10
//   }