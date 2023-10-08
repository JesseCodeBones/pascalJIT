#ifndef __pascal_jit_runtime__
#define __pascal_jit_runtime__

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>


void runtime_write_string(char* str) {
  printf("%s", str);
}
void runtime_writeln_string(char* str) {
  printf("%s\n", str);
}

void runtime_write_int(int value) {
  printf("%d", value);
}
void runtime_writeln_int(int value) {
  printf("%d\n", value);
}

static std::unique_ptr<std::stack<uint64_t>> runtimeStack = std::make_unique<std::stack<uint64_t>>();

static void pushStack(uint64_t value){
  runtimeStack->push(value);
}

static uint64_t popStack(uint64_t value){
  auto result = runtimeStack->top();
  runtimeStack->pop();
  return result;
}

class Runtime{
public:

  Runtime(){
    nativeFunction["write_string"] = (void*)runtime_write_string;
    nativeFunction["writeln_string"] = (void*) runtime_writeln_string;
    nativeFunction["write_int"] = (void*) runtime_write_int;
    nativeFunction["writeln_int"] = (void*) runtime_writeln_int;
    nativeFunction["__runtime_push__"] = (void*) pushStack;
    nativeFunction["__runtime_pop__"] = (void*) popStack;
    stringLiterals.reserve(1024*1024*8);
  }
  
  const char* addStringLiteral(std::string str) {
    auto iterator = std::find(stringLiterals.cbegin(), stringLiterals.cend(), str);
    if(iterator == stringLiterals.cend()) {
      stringLiterals.push_back(str);
      return stringLiterals.back().c_str();
    } else {
      return (*iterator).c_str();
    }
    
  }

  std::vector<std::string> stringLiterals;
  std::unordered_map<std::string, void*> nativeFunction;
};

#endif