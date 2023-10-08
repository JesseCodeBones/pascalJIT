#ifndef __pascal_jit_AST__
#define __pascal_jit_AST__
#include "./Assembly.hpp"
#include "./Runtime.hpp"
#include "./Token.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static uint32_t codeGenScopeIndex = 0U;

#ifdef asm_debug
#define DEBUG(STR)                                                       \
    std::cout << STR << std::endl;                                            
#else
#define DEBUG(STR) {}
#endif

class BaseAST {
public:
  virtual std::vector<uint8_t> codegen() = 0;
};
static std::unique_ptr<Runtime> runtimePtr = std::make_unique<Runtime>();
class ExpressionAST : public BaseAST {
public:
  Token type;
  virtual std::vector<uint8_t> codegen() = 0;
  virtual ~ExpressionAST(){};
};

class BinaryExpressionAST : public ExpressionAST {

public:
std::unique_ptr<ExpressionAST> LHS;
Token op;
std::unique_ptr<ExpressionAST> RHS;

virtual std::vector<uint8_t> codegen() override {
  std::vector<uint8_t> result;
  return result;
}
};

class StringLiteralExpressionAST : public ExpressionAST {
public:
  StringLiteralExpressionAST(std::string literal) : literal(literal) {
    type = Token::tok_string;
  }
  ~StringLiteralExpressionAST() override = default;
  std::string literal;
  virtual std::vector<uint8_t> codegen() override {
    /// move ptr to R9
    std::vector<uint8_t> result;
    const char *strPtr = runtimePtr->addStringLiteral(literal);
    addAssemblyToExecutable(result, insertPtrToRegister(9, strPtr));
    DEBUG("addAssemblyToExecutable(executable, insertPtrToRegister(9," << std::hex << static_cast<const void* const>(strPtr)<<"));");
    return result;
  }
};

class IntegerLiteralExpressionAST: public ExpressionAST {
public:
  IntegerLiteralExpressionAST(int literal) : literal(literal) {
    type = Token::tok_integer;
  }
  ~IntegerLiteralExpressionAST() override = default;
  int literal;
  virtual std::vector<uint8_t> codegen() override {
    /// move ptr to R9
    std::vector<uint8_t> result;
    addAssemblyToExecutable(result, insertIntegerToRegister(9, literal));
    DEBUG("addAssemblyToExecutable(executable, insertIntegerToRegister(9," << std::dec << literal<<"));");
    return result;
  }
};

class IdentifierExpressionAST : public ExpressionAST {
public:
  std::string identifier;
  uint32_t index;
  virtual std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> result;
    addAssemblyToExecutable(result, sub_register_imm(10, 29, 0x8*index));
    DEBUG("addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x"<<std::hex<<0x8*index<<"));");
    addAssemblyToExecutable(result,
                            ldr_register_register_offset(9, 10, 0));
    DEBUG("addAssemblyToExecutable(executable, ldr_register_register_offset(9, 10, 0));");
    return result;
  }
};

class UnaryExpressionAST : public ExpressionAST {
public:
  Token op;
  std::unique_ptr<ExpressionAST> operand;
  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> result;
    addAssemblyToExecutable(result, operand->codegen());
    if(op == Token::tok_neg) {
      addAssemblyToExecutable(result, neg_register_register_32(9, 9));
      DEBUG("addAssemblyToExecutable(executable, neg_register_register_32(9, 9));");
    }
    return result;
  }
};

class AssignmentExpressionAST: public ExpressionAST {
  public:
  std::string variableName;
  std::unique_ptr<ExpressionAST> assignment;
  uint32_t scopeIndex = 1; // from 1 to n
  std::vector<uint8_t> codegen() override {
    if(!assignment) {
      std::cout << "illegal assignment expression\n";
      throw new std::runtime_error("illegal assignment expression");
    }
    std::vector<uint8_t> executable;
    addAssemblyToExecutable(executable, assignment->codegen()); // mov value to R9
    addAssemblyToExecutable(executable, sub_register_imm(10, 29, scopeIndex*0x8));
    DEBUG("addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x" << std::hex <<scopeIndex*0x8<<"));");
    addAssemblyToExecutable(executable, str_register_register_offset(
                                            9, 10, 0));
    DEBUG("addAssemblyToExecutable(executable, str_register_register_offset(9, 10, 0));");
    return executable;
  }
};

// TODO find call type with signature of callee
class CallExpressionAST : public ExpressionAST {
public:
  CallExpressionAST(std::string calleeName) : calleeName(calleeName) {}
  ~CallExpressionAST() override = default;
  std::string calleeName;
  std::vector<std::unique_ptr<ExpressionAST>> args;
  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> result;

    uint32_t regIndex = 0;
    std::stringstream runtimeFunSigName;
    for (auto &arg : args) {
      switch (arg->type){
        case Token::tok_string: {
          runtimeFunSigName << "_string";
          break;
        }
        case Token::tok_integer: {
          runtimeFunSigName << "_int";
          break;
        }
        default:
          break;
      }
      auto argToR9 = arg->codegen();
      addAssemblyToExecutable(result, argToR9);
      addAssemblyToExecutable(result, mov_register_register(regIndex, 9));
      DEBUG("addAssemblyToExecutable(executable, mov_register_register("<<regIndex<<", 9));");
      regIndex++;
    }
    std::stringstream runtimeFunc;
    runtimeFunc << calleeName;
    runtimeFunc << runtimeFunSigName.str();
    void *funPtr = runtimePtr->nativeFunction[runtimeFunc.str()];
    if(!funPtr) {
      throw new std::runtime_error("cannot find runtime function:"+runtimeFunc.str());
    }
    addAssemblyToExecutable(result, insertPtrToRegister(9, funPtr));
    DEBUG("addAssemblyToExecutable(executable, insertPtrToRegister(9, 0x"<<std::hex << funPtr<<"));");
    addAssemblyToExecutable(result, callRegister(9));
    DEBUG("addAssemblyToExecutable(executable, callRegister(9));");

    return result;
  }
};

class VariableAST : public ExpressionAST {
public:
  std::string variableName;
  std::unique_ptr<ExpressionAST> assignment;
  uint32_t scopeIndex = 1; // from 1 to n
  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> executable;
    if (scopeIndex % 2 > 0) { // only move sp 0x10
      addAssemblyToExecutable(executable, sub_register_imm(31, 31, 0x10));
      DEBUG("addAssemblyToExecutable(executable, sub_register_imm(31, 31, 0x10));");
    }
    if (assignment) {
      addAssemblyToExecutable(executable, assignment->codegen()); // mov value to R9
      addAssemblyToExecutable(executable, sub_register_imm(10, 29, scopeIndex*0x8));
      DEBUG("addAssemblyToExecutable(executable, sub_register_imm(10, 29, 0x" << std::hex <<scopeIndex*0x8<<"));");
      addAssemblyToExecutable(executable, str_register_register_offset(
                                              9, 10, 0));
      DEBUG("addAssemblyToExecutable(executable, str_register_register_offset(9, 10, 0));");
    }
    return executable;
  }
};

class ProgramAST : public BaseAST {
public:
  ProgramAST(std::string name) : programName(name) {}
  std::string programName;
  std::vector<std::unique_ptr<ExpressionAST>> topLevelExpressions;
  uint32_t scopeIndex;

  std::vector<uint8_t> codegen() override {
    std::vector<uint8_t> executable;
    codeGenScopeIndex = scopeIndex;
    addAssemblyToExecutable(executable, storeX29X30());
    DEBUG("addAssemblyToExecutable(executable, storeX29X30());");
    addAssemblyToExecutable(executable, mov_register_register_from_to_sp(29, 31));
    DEBUG("addAssemblyToExecutable(executable, mov_register_register_from_to_sp(29, 31));");
    for (auto &toplevelExpression : topLevelExpressions) {
      const std::vector<uint8_t> &expressionExecutable =
          toplevelExpression->codegen();
      std::copy(expressionExecutable.begin(), expressionExecutable.end(),
                std::back_inserter(executable));
    }
    addAssemblyToExecutable(executable, mov_register_register_from_to_sp(31, 29));
    DEBUG("addAssemblyToExecutable(executable, mov_register_register_from_to_sp(31, 29));");
    addAssemblyToExecutable(executable, loadX29X30());
    DEBUG("addAssemblyToExecutable(executable, loadX29X30());");
    addAssemblyToExecutable(executable, ret());
    DEBUG("addAssemblyToExecutable(executable, ret());");
    return executable;
  }
};

#endif