#ifndef __pascal_jit_AST__
#define __pascal_jit_AST__
#include "./Assembly.hpp"
#include "./Runtime.hpp"
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
    runtimePtr->addStringLiteral(literal);
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
    addAssemblyToExecutable(result, storeX29X30());
    uint32_t regIndex = 0;
    for (auto &arg : args) {
      if (dynamic_cast<StringLiteralExpressionAST *>(arg.get())) {
        // string literal argument
        std::unique_ptr<StringLiteralExpressionAST> stringPtr(
            static_cast<StringLiteralExpressionAST *>(arg.release()));
        stringPtr->runtimePtr = runtimePtr;
        stringPtr->codegen();
        auto it = std::find_if(
          stringPtr->runtimePtr->stringLiterals.begin(), 
          stringPtr->runtimePtr->stringLiterals.end(), 
          [&stringPtr](const std::string& str){return str == stringPtr->literal;});
        if (it != stringPtr->runtimePtr->stringLiterals.end()) {
          addAssemblyToExecutable(result,
                                insertPtrToRegister(regIndex, (void*)(*it).c_str()));
        }
      }
      regIndex++;
    }

    void *funPtr = runtimePtr->nativeFunction[calleeName];
    addAssemblyToExecutable(result, insertPtrToRegister(9, funPtr));
    addAssemblyToExecutable(result, callRegister(9));
    addAssemblyToExecutable(result, loadX29X30());
    addAssemblyToExecutable(result, ret());
    return result;
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