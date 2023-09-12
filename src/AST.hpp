#ifndef __pascal_jit_AST__
#define __pascal_jit_AST__
#include "./Assembly.hpp"
#include "./Runtime.hpp"
#include "./Token.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class BaseAST {
public:
  virtual std::vector<uint8_t> codegen() = 0;
  std::shared_ptr<Runtime> runtimePtr;
};

class ExpressionAST : public BaseAST {
public:
  virtual std::vector<uint8_t> codegen() = 0;
  virtual ~ExpressionAST(){};
};

class StringLiteralExpressionAST : public ExpressionAST {
public:
  StringLiteralExpressionAST(std::string literal) : literal(literal) {}
  ~StringLiteralExpressionAST() override = default;
  std::string literal;
  virtual std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> empty;
    return empty;
  }
};

class IdentifierExpressionAST : public ExpressionAST {
public:
  std::string identifier;
  virtual std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> empty;
    return empty;
  }
};

class CallExpressionAST : public ExpressionAST {
public:
  CallExpressionAST(std::string calleeName) : calleeName(calleeName) {}
  ~CallExpressionAST() override = default;
  std::string calleeName;
  std::vector<std::unique_ptr<ExpressionAST>> args;
  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> result;

    uint32_t regIndex = 0;
    for (auto &arg : args) {
      if (dynamic_cast<StringLiteralExpressionAST *>(arg.get())) {
        // string literal argument
        std::unique_ptr<StringLiteralExpressionAST> stringPtr(
            static_cast<StringLiteralExpressionAST *>(arg.release()));
        const char *strPtr = runtimePtr->addStringLiteral(stringPtr->literal);
        addAssemblyToExecutable(result, insertPtrToRegister(regIndex, strPtr));
      }

      regIndex++;
    }

    void *funPtr = runtimePtr->nativeFunction[calleeName];
    addAssemblyToExecutable(result, insertPtrToRegister(9, funPtr));
    addAssemblyToExecutable(result, callRegister(9));

    return result;
  }
};

class VariableAST : public ExpressionAST {
public:
  std::string variableName;
  Token type;
  std::unique_ptr<ExpressionAST> assignment;
  uint32_t scopeIndex = 1; // from 1 to n
  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> executable;
    if (scopeIndex % 2 > 0) { // only move sp 0x10
      addAssemblyToExecutable(executable, sub_register_imm(31, 31, 0x10));
    }
    if (assignment) {
      if (dynamic_cast<StringLiteralExpressionAST *>(assignment.get())) {
        // string literal assignment
        std::unique_ptr<StringLiteralExpressionAST> stringVal(
            static_cast<StringLiteralExpressionAST *>(assignment.release()));
        const char *strPtr = runtimePtr->addStringLiteral(stringVal->literal);
        addAssemblyToExecutable(executable, insertPtrToRegister(9, strPtr));
        addAssemblyToExecutable(executable, str_register_register_offset(
                                                9, 31, (scopeIndex % 2) * 0x8));
      }
    }
    return executable;
  }
};

class ProgramAST : public BaseAST {
public:
  ProgramAST(std::string name) : programName(name) {}
  std::string programName;
  std::vector<std::unique_ptr<ExpressionAST>> topLevelExpressions;

  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> executable;
    addAssemblyToExecutable(executable, storeX29X30());
    addAssemblyToExecutable(executable, mov_register_register(29, 31));
    for (auto &toplevelExpression : topLevelExpressions) {
      const std::vector<uint8_t> &expressionExecutable =
          toplevelExpression->codegen();
      std::copy(expressionExecutable.begin(), expressionExecutable.end(),
                std::back_inserter(executable));
    }
    addAssemblyToExecutable(executable, mov_register_register(9, 31));
    addAssemblyToExecutable(executable, sub_register_register(9, 29, 9));
    addAssemblyToExecutable(executable, add_register_register(31, 31, 9));
    addAssemblyToExecutable(executable, loadX29X30());
    addAssemblyToExecutable(executable, ret());
    return executable;
  }
};

#endif