#ifndef __pascal_jit_parser__
#define __pascal_jit_parser__
#include "AST.hpp"
#include "Runtime.hpp"
#include "Token.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

#define DEBUG_TOKEN(STR)                                                       \
  if (debug) {                                                                 \
    if (STR > 0) {                                                             \
      std::cout << (char)STR << std::endl;                                     \
    } else {                                                                   \
      std::cout << STR << std::endl;                                           \
    }                                                                          \
  }


static uint32_t currentScope = 0U;
static std::unordered_map<uint32_t, std::unordered_map<std::string, uint32_t>> scopeLocals;
static std::unordered_map<uint32_t, std::unordered_map<std::string, Token>> scopeIdentifierTypes;

class Parser {

public:
  Parser(Tokenizer &tokenizer, std::shared_ptr<Runtime> runtime)
      : tokenizer(tokenizer) {}

  std::unique_ptr<ExpressionAST> parseIdentifierExpression() {
    std::string identifier = tokenizer.identifier;
    getNextToken(); // eat identifier
    if (currentToken == '(') {
      std::unique_ptr<CallExpressionAST> callAST =
          std::make_unique<CallExpressionAST>(identifier);
      getNextToken(); // eat (
      while (currentToken != ')') {
        auto expression = parseExpression();
        callAST->args.push_back(std::move(expression));
      }
      return std::move(callAST);
    } else if (currentToken == Token::tok_assign) {
      getNextToken(); // eat :=
      std::unique_ptr<AssignmentExpressionAST> assignment = std::make_unique<AssignmentExpressionAST>();
      assignment->assignment = parseExpression();
      assignment->variableName = identifier;
      assignment->scopeIndex = scopeLocals[currentScope][identifier];
      return std::move(assignment);
    } else {
      // normal identifier expression
      std::unique_ptr<IdentifierExpressionAST> identifierPtr = std::make_unique<IdentifierExpressionAST>();
      identifierPtr->identifier = identifier;
      identifierPtr->type = scopeIdentifierTypes[currentScope][identifier];
      identifierPtr->index = scopeLocals[currentScope][identifier];
      return std::move(identifierPtr);
    }

    return nullptr;
  }

  std::unique_ptr<ExpressionAST> parseUnaryExpression() {
    std::unique_ptr<UnaryExpressionAST> unary = std::make_unique<UnaryExpressionAST>();
    unary->op = static_cast<Token>(currentToken);
    getNextToken(); // eat op
    unary->operand = std::move(parseExpression());
    unary->type = unary->operand->type;
    if(unary->operand) {
      return std::move(unary);
    } else {
      return nullptr;
    }
  }

  std::unique_ptr<ExpressionAST> parseBinaryExpression(){
    getNextToken(); // eat (
    std::unique_ptr<BinaryExpressionAST> binary = std::make_unique<BinaryExpressionAST>();
    binary->LHS = parseExpression();
    if(currentToken == ')') {
      // no RHS, eat ) and return LHS
      getNextToken();
      return std::move(binary->LHS);
    } else if (currentToken == Token::tok_positive || currentToken == Token::tok_neg) {
      // currently support pos and neg
      Token op = static_cast<Token>(currentToken);
      binary->op = op;
      getNextToken(); // eat token
      binary->RHS = parseExpression();
      if(currentToken != ')'){
        std::cout << "binary expression without )\n";
        throw std::runtime_error("binary expression without )");
      } else {
        getNextToken(); // eat )
      }
      return std::move(binary);
    } else {
      std::cout << "Illegal binary expression\n";
      throw std::runtime_error("Illegal binary expression");
    }
  }

  std::unique_ptr<ExpressionAST> parseExpression() {
    // parse call
    switch (currentToken) {
    case tok_identifier:
      return parseIdentifierExpression();
    case tok_string_literal: {
      std::string strValue = tokenizer.stringLiteral;
      getNextToken();
      return std::make_unique<StringLiteralExpressionAST>(strValue);
    }
    case tok_number: {
      int value = tokenizer.numberVal;
      getNextToken();
      return std::make_unique<IntegerLiteralExpressionAST>(value);
    }
    case tok_neg: {
      return std::move(parseUnaryExpression());
    }
    case Token::tok_var: {
        auto varExpression = parseVariableExpression();
        if (varExpression) {
          return std::move(varExpression);
        }
        return nullptr;
      }
    case '(': {
      // probe binary expression
      return std::move(parseBinaryExpression());
    }
    default:
      getNextToken(); // eat undefined token
      return nullptr;
    }
  }

  std::unique_ptr<ExpressionAST> parseVariableExpression() {
    getNextToken(); // eat var
    if (currentToken != Token::tok_identifier) {
      throw std::runtime_error("var without identifier");
    }
    std::unique_ptr<VariableAST> variable = std::make_unique<VariableAST>();
    variable->variableName = tokenizer.identifier;
    // currentLocals++;
    auto& locals = scopeLocals[currentScope];
    if(locals.find(variable->variableName) != locals.cend()) {
      throw std::runtime_error("duplicate variable");
    }
    auto index = locals.size() + 1U;
    locals.emplace(variable->variableName, index);
    variable->scopeIndex = index;
    
    getNextToken(); // eat identifier
    if (currentToken != Token::tok_assign_type) {
      throw std::runtime_error("var without type");
    }
    getNextToken();                          // eat :
    if (currentToken != Token::tok_string && currentToken != Token::tok_integer) { // if not type token
      throw std::runtime_error("var with incorrect type");
    } else {
      // handle type
      variable->type = (Token)currentToken;
    }
    auto& identifierTypes = scopeIdentifierTypes[currentScope];
    identifierTypes.emplace(variable->variableName, variable->type);

    getNextToken(); // eat type
    if (currentToken == '=') {
      getNextToken();
      // handle literal
      variable->assignment = parseExpression();
    }
    return std::move(variable);
  }

  std::vector<std::unique_ptr<ExpressionAST>> parseTopLevelExpression() {
    // parse call expression
    std::vector<std::unique_ptr<ExpressionAST>> result;
    
    while (true) {
      switch (currentToken) {
      case Token::tok_eof: {
        return result;
      }
      case ';': {
        getNextToken();
        break;
      }
      
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
    currentScope = currentScope + 1;
    auto program = std::make_unique<ProgramAST>(programName);
    program->scopeIndex = currentScope;
    getNextToken(); // eat programName
    getNextToken(); // escape ;
    while (currentToken != Token::tok_begin) {
      std::unique_ptr<ExpressionAST> topLevelIdentification =
        parseExpression();
      if(topLevelIdentification) {
        program->topLevelExpressions.push_back(std::move(topLevelIdentification));
      }
    }
    getNextToken(); // eat begin
    std::vector<std::unique_ptr<ExpressionAST>> topLevelExpressions =
        parseTopLevelExpression();
    for (auto &expressionPtr : topLevelExpressions) {
      program->topLevelExpressions.push_back(std::move(expressionPtr));
    }
    return std::move(program);
  }

  std::unique_ptr<ProgramAST> parse() {
    getNextToken();
    std::unique_ptr<ProgramAST> program;
    while (true) {
      switch (currentToken) {
      case Token::tok_eof:
        return std::move(program);
      case ';':
        getNextToken();
        break;
      case Token::tok_program: {
        program = std::move(parseProgram());
        break;
      }
      }
    }
  }

  bool debug = false;

private:
  int currentToken;
  Tokenizer &tokenizer;
  int getNextToken() {
    currentToken = tokenizer.getToken();
    DEBUG_TOKEN(currentToken);
    return currentToken;
  }
};

#endif