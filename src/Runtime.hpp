#ifndef __pascal_jit_runtime__
#define __pascal_jit_runtime__

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


void runtime_write_string(char* str) {
  printf("%s", str);
}
void runtime_writeln_string(char* str) {
  printf("%s\n", str);
}

class Runtime{
public:

  Runtime(){
    nativeFunction["write_string"] = (void*)runtime_write_string;
    nativeFunction["writeln_string"] = (void*) runtime_writeln_string;
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