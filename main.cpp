#include <iostream>
#include <filesystem>
#include <fstream>
#include "./src/Token.hpp"
#include "./src/Parser.hpp"
#include "./src/JIT.hpp"
int main(int argc, char ** argv) {

if(argc != 2U) {
    return 1; 
}

char* pascal = argv[1];

    std::ifstream t(pascal);
    std::stringstream buffer;
    buffer << t.rdbuf();
    buffer << EOF;

    Tokenizer tokenizer(buffer.str());
    Parser parser(tokenizer, std::make_shared<Runtime>());
    auto program = parser.parse();

    auto fun = createJit(program->codegen());
    fun();
}
