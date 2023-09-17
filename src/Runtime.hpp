#ifndef __pascal_jit_runtime__
#define __pascal_jit_runtime__

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" {

  void runtime_write(char* str) {
    printf("%s", str);
  }
}

class Runtime{
public:

  Runtime(){
    nativeFunction["write"] = (void*)runtime_write;
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