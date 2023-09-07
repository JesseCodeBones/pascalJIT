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
  

  //literal
  tok_string_literal = -9,

  tok_dot = -10,

};

class Tokenizer {

public:
  Tokenizer(std::string _source) : source(_source) {}
  int getToken() {

    if (_lastChar == EOF || _pos >= source.size()) {
      return Token::tok_eof;
    }
    while (isspace(_lastChar)) {
      _lastChar = source.at(_pos++);
    }

    // single line comments
    if ('{' == _lastChar) {
      do {
        _lastChar = source.at(_pos++);
        if (_lastChar == EOF || _lastChar == '\n' || _lastChar == '\r') {
          throw std::runtime_error("illegal comments");
        }
      } while (_lastChar != '}');
      _lastChar = source.at(_pos++);
    }

    if (isalpha(_lastChar)) {
      identifier = _lastChar;
      while (isalpha(_lastChar = source.at(_pos++))) {
        identifier.push_back(_lastChar);
      }

      if (identifier == "program") {
        return Token::tok_program;
      }

      if (identifier == "begin") {
        return Token::tok_begin;
      }

      if (identifier == "end") {
        return Token::tok_end;
      }
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

    if(_lastChar == '\'') {
      _lastChar = source.at(_pos++);
      stringLiteral = "";
      while (_lastChar != '\'') {
        stringLiteral.push_back(_lastChar);
        _lastChar = source.at(_pos++);
      }
      _lastChar = source.at(_pos++);
      return Token::tok_string_literal;
    }

    if(_lastChar == '.') {
      _lastChar = source.at(_pos++);
      return Token::tok_dot;
    }

    int thisChar = _lastChar; ///< very important
    _lastChar = source.at(_pos++);
    return thisChar;
  }

  std::string identifier;
  uint32_t uSignNumber = 0U;
  std::string stringLiteral;

private:
  std::string source;
  char _lastChar = ' ';
  uint32_t _pos = 0U;
};

#endif