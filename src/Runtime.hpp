#ifndef __pascal_jit_runtime__
#define __pascal_jit_runtime__

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" {

  void runtime_write(char* str) {
    printf(str);
  }
}

class Runtime{
public:

  Runtime(){
    nativeFunction["write"] = (void*)runtime_write;
  }
  
  void addStringLiteral(std::string str) {
    stringLiterals.push_back(str);
  }

  std::vector<std::string> stringLiterals;
  std::unordered_map<std::string, void*> nativeFunction;
};

#endif