#ifndef __pacal_jit_token__
#define __pacal_jit_token__

#include <cstdint>
#include <string>
#include <cctype>

class Token : uint32_t{

    tok_eof = -1,

    // primary
    tok_identifier = -2,
    tok_number = -3,
};

class Tokenizer {

public:
    Tokenizer(std::string _source):source(_source){
        while(isSpace(_lastChar)) {
            _lastChar = source.at(_pos);
        }
    }

private:
    std::string source;
    char _lastChar = ' ';
    uint32_t _pos = 0U;
};

#endif