#ifndef __pascal_jit_runtime__
#define __pascal_jit_runtime__

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class Runtime{
public:

  Runtime(){
    nativeFunction["write"] = (void*)printf;
  }
  
  void addStringLiteral(std::string& str) {
    stringLiterals.emplace(str, std::make_shared<std::string>(str));
  }

  std::unordered_map<std::string, std::shared_ptr<std::string>> stringLiterals;
  std::unordered_map<std::string, void*> nativeFunction;
};

#endif