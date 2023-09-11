#include <cstdio>
#include <cstring>
#include <filesystem>
#include <functional>
#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "../src/Token.hpp"
#include "../src/Parser.hpp"
#include "../src/JIT.hpp"
#include "../src/Runtime.hpp"
#include "../src/Assembly.hpp"

const std::filesystem::path workspace = std::filesystem::current_path().parent_path().parent_path();

TEST(compiler_e2e, base) { 

    std::ifstream t(workspace / "tests" / "cases" / "helloworld.pas");
    std::stringstream buffer;
    buffer << t.rdbuf();
    buffer << EOF;

    Tokenizer tokenizer(buffer.str());
    Parser parser(tokenizer, std::make_shared<Runtime>());
    auto program = parser.parse();

    auto fun = createJit(program->codegen());
    fun();
    // std::vector<uint8_t> result;
    // char* testString = "hello JIT\n";
    // addAssemblyToExecutable(result, storeX29X30());
    // addAssemblyToExecutable(result, insertPtrToRegister(0, testString));
    // Runtime runtime;
    // void* writter = runtime.nativeFunction["write"];
    // addAssemblyToExecutable(result, insertPtrToRegister(9, writter));
    // addAssemblyToExecutable(result, callRegister(9));
    // addAssemblyToExecutable(result, loadX29X30());
    // addAssemblyToExecutable(result, ret());
    // auto fun = createJit(result);
    // fun();
    
 }