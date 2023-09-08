#include <cstdio>
#include <cstring>
#include <filesystem>
#include <functional>
#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "../src/Token.hpp"
#include "../src/Parser.hpp"

const std::filesystem::path workspace = std::filesystem::current_path().parent_path().parent_path();

TEST(compiler_e2e, base) { 

    std::ifstream t(workspace / "tests" / "cases" / "helloworld.pas");
    std::stringstream buffer;
    buffer << t.rdbuf();
    buffer << EOF;

    Tokenizer tokenizer(buffer.str());
    Parser parser(tokenizer);
    parser.parse();
    


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