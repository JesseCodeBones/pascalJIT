#include "../src/Assembly.hpp"
#include "../src/JIT.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime.hpp"
#include "../src/Token.hpp"
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

TEST(compiler_e2e, parser) {
  std::ifstream t(workspace / "tests" / "cases" / "printVariable.pas");
  std::stringstream buffer;
  buffer << t.rdbuf();
  Tokenizer tokenizer(buffer.str());
  Parser parser(tokenizer, std::make_shared<Runtime>());
  parser.debug = true;
  auto program = parser.parse();
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