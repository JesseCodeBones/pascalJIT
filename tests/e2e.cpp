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


    // auto nextTok = tokenizer.getToken();

    // while ( nextTok != Token::tok_eof) {
    //     if(nextTok<0) {
    //         std::cout << nextTok << std::endl;
    //     } else {
    //         std::cout << static_cast<char>(nextTok) << std::endl;
    //     }
        
    //     nextTok = tokenizer.getToken();
    // }
    
 }