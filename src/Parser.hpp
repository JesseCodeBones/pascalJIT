#ifndef __pascal_jit_parser__
#define __pascal_jit_parser__
#include "AST.hpp"
#include "Token.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>

class Parser {

public:
  Parser(Tokenizer &tokenizer) : tokenizer(tokenizer) {}

  std::unique_ptr<ExpressionAST> parseIdentifierExpression() {
    std::string identifier = tokenizer.identifier;
    getNextToken(); // eat identifier
    if (currentToken == '(') {
      std::unique_ptr<CallExpressionAST> callAST =
          std::make_unique<CallExpressionAST>(identifier);
      getNextToken(); // eat (
      while (currentToken != ')') {
        switch (currentToken) {
        case Token::tok_string_literal: {
          std::string strValue = tokenizer.stringLiteral;
          std::unique_ptr<StringLiteralExpressionAST> stringLiteral =
              std::make_unique<StringLiteralExpressionAST>(strValue);
          callAST->args.push_back(std::move(stringLiteral));
          getNextToken();
          break;
        }
        default:
          getNextToken();
          break;
        }
      }
      return std::move(callAST);
    }

    return nullptr;
  }

  std::unique_ptr<ExpressionAST> parseExpression() {
    // parse call
    switch (currentToken) {
    case tok_identifier:
      return parseIdentifierExpression();
    default:
      getNextToken(); // eat undefined token
      return nullptr;
    }
  }

  std::vector<std::unique_ptr<ExpressionAST>> parseTopLevelExpression() {
    // parse call expression
    std::vector<std::unique_ptr<ExpressionAST>> result;
    while (true) {
      switch (currentToken) {
      case Token::tok_eof: {
        return result;
      }
      case ';':
        getNextToken();
        break;
      default: {
        auto expression = parseExpression();
        if (expression) {
          result.push_back(std::move(expression));
        }
      }
      }
    }
  }

  std::unique_ptr<ProgramAST> parseProgram() {

    getNextToken();
    if (currentToken != Token::tok_identifier) {
      throw std::runtime_error("program does not have a name");
    }
    const std::string programName = tokenizer.identifier;
    getNextToken(); // eat programName
    getNextToken(); // escape ;
    getNextToken(); // eat begin
    std::vector<std::unique_ptr<ExpressionAST>> topLevelExpressions =
        parseTopLevelExpression();
    auto program = std::make_unique<ProgramAST>(programName);
    for (auto &expressionPtr : topLevelExpressions) {
      program->topLevelExpressions.push_back(std::move(expressionPtr));
    }
    return std::move(program);
  }

  void parse() {
    getNextToken();
    while (true) {
      switch (currentToken) {
      case Token::tok_eof:
        return;
      case ';':
        getNextToken();
        break;
      case Token::tok_program:
        parseProgram();
        break;
      }
    }
  }

private:
  int currentToken;
  Tokenizer &tokenizer;
  int getNextToken() {
    currentToken = tokenizer.getToken();
    return currentToken;
  }
};

#endif