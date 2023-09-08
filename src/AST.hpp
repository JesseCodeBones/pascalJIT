#ifndef __pascal_jit_AST__
#define __pascal_jit_AST__
#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class BaseAST {

  virtual std::vector<uint8_t> codegen() = 0;
};

class ExpressionAST : public BaseAST {
public:
  virtual std::vector<uint8_t> codegen() = 0;
};

class StringLiteralExpressionAST : public ExpressionAST {
public:
  StringLiteralExpressionAST(std::string literal) : literal(literal) {}
  std::string literal;
  std::vector<uint8_t> codegen() override {
    
  }
};

class CallExpressionAST : public ExpressionAST {
public:
  CallExpressionAST(std::string calleeName)
      : calleeName(calleeName) {}

  std::string calleeName;
  std::vector<std::unique_ptr<ExpressionAST>> args;
  std::vector<uint8_t> codegen() override {
    
  }
};

class ProgramAST : public BaseAST {
public:
  ProgramAST(std::string name) : programName(name) {}
  std::string programName;
  std::vector<std::unique_ptr<ExpressionAST>> topLevelExpressions;

  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> executable;
    for (auto &toplevelExpression : topLevelExpressions) {
      const std::vector<uint8_t> &expressionExecutable =
          toplevelExpression->codegen();
      std::copy(expressionExecutable.begin(), expressionExecutable.end(),
                std::back_inserter(executable));
    }
    return executable;
  }
};

#endif