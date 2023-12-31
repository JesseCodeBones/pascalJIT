#ifndef __pascal_jit_token__
#define __pascal_jit_token__

#include <cassert>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <sstream>
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
  tok_var = -6,

  // control
  tok_begin = -7,
  tok_end = -8,

  // literal
  tok_string_literal = -9,

  tok_dot = -10,

  // type
  tok_string = -11,
  tok_integer = -12,

  tok_neg = -13,
  tok_positive = -14,

  tok_assign = -15,
  tok_assign_type = -16,

  tok_result = -17,
  tok_if = -18,

  tok_eq = -19,
};

class Tokenizer {

public:
  Tokenizer(std::string _source) : source(_source) {}
  int getToken() {
    if (_pos >= source.size()) {
      return Token::tok_eof;
    }
    while (isspace(_lastChar)) {
      nextChar();
    }
    // single line comments
    if ('{' == _lastChar) {
      do {
        nextChar();
        if (_pos >= source.size() || _lastChar == '\n' || _lastChar == '\r') {
          throw std::runtime_error("illegal comments");
        }
      } while (_lastChar != '}');
      nextChar();
      return getToken();
    }

    if (_lastChar == ':') {
      nextChar();
      if (_lastChar == '=') {
        nextChar();
        return Token::tok_assign;
      } else {
        return Token::tok_assign_type;
      }
    }

    if (isalpha(_lastChar)) {
      std::stringstream stream;
      stream << _lastChar;
      while (isalpha(_lastChar = source.at(_pos++))) {
        stream << _lastChar;
      }

      identifier = stream.str();

      if (identifier == "program") {
        return Token::tok_program;
      }

      if (identifier == "Result") {
        return Token::tok_result;
      }

      if (identifier == "begin") {
        return Token::tok_begin;
      }

      if (identifier == "end") {
        return Token::tok_end;
      }
      if (identifier == "String") {
        return Token::tok_string;
      }
      if (identifier == "Integer") {
        return Token::tok_integer;
      }
      if (identifier == "var") {
        return Token::tok_var;
      }
      if (identifier == "if") {
        return Token::tok_if;
      }
      return Token::tok_identifier;
    }

    if (isdigit(_lastChar)) {
      std::string digitString;
      digitString.push_back(_lastChar);
      while (isdigit(_lastChar = source.at(_pos++))) {
        digitString.push_back(_lastChar);
      }
      numberVal = std::stoi(digitString);
      return Token::tok_number;
    }

    if (_lastChar == '\'') {
      nextChar();
      stringLiteral = "";
      std::stringstream stream;
      while (_lastChar != '\'') {
        if (_lastChar == '\\') {
          nextChar();
          if (_lastChar == 'n') {
            stream << '\n';
            nextChar();
          }
          continue; // escape '\'
        }
        stream << _lastChar;
        nextChar();
      }
      stringLiteral = stream.str();
      nextChar();
      return Token::tok_string_literal;
    }

    if (_lastChar == '.') {
      nextChar();
      return Token::tok_dot;
    }

    if (_lastChar == '-') {
      nextChar();
      return Token::tok_neg;
    }
    if (_lastChar == '+') {
      nextChar();
      return Token::tok_positive;
    }
    if (_lastChar == '=') {
      nextChar();
      return Token::tok_eq;
    }

    int thisChar = _lastChar; ///< very important
    nextChar();
    return thisChar;
  }

  std::string identifier;
  int numberVal = 0U;
  std::string stringLiteral;

private:
  std::string source;
  char _lastChar = ' ';
  uint32_t _pos = 0U;
  void nextChar() {
    if (_pos >= source.size()) {
      _lastChar = Token::tok_eof;
    } else {
      _lastChar = source.at(_pos++);
    }
  }
};

#endif