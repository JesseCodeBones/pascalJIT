#ifndef __pacal_jit_token__
#define __pacal_jit_token__

#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>

enum Token : int {

  tok_eof = -1,

  // primary
  tok_identifier = -2,
  tok_number = -3,

  // command
  tok_program = -4,
  tok_function = -5,

  // type
  tok_integer = -6,

  // control
  tok_begin = -7,
  tok_end = -8,

};

class Tokenizer {

public:
  Tokenizer(std::string _source) : source(_source) {}
  int getToken() {
    while (isspace(_lastChar)) {
      _lastChar = source.at(_pos++);
    }

    if (isalpha(_lastChar)) {
      identifier = _lastChar;
      while (isalpha(_lastChar = source.at(_pos++))) {
        identifier.push_back(_lastChar);
      }

      //   if (identifier == "def") {
      //     return Token::tok_def;
      //   }
      return Token::tok_identifier;
    }

    if (isdigit(_lastChar)) {
      std::string digitString;
      digitString.push_back(_lastChar);
      while (isdigit(_lastChar = source.at(_pos++))) {
        digitString.push_back(_lastChar);
      }
      uSignNumber = std::stoul(digitString);
      return Token::tok_number;
    }

    // single line comments
    if ('{' == _lastChar) {
      do {
        _lastChar = source.at(_pos++);
        if(_lastChar == EOF || _lastChar == '\n' || _lastChar == '\r' ) {
            throw std::runtime_error("illegal comments");
        }
      } while (_lastChar != '}');
    }

    return _lastChar;
  }

  std::string identifier;
  uint32_t uSignNumber = 0U;

private:
  std::string source;
  char _lastChar = ' ';
  uint32_t _pos = 0U;
};

#endif