#ifndef __pascal_jit_JIT__
#define __pascal_jit_JIT__
#include <functional>
#include <sys/mman.h>
#include <cstring>
#include <cstdint>

static std::function<void()> createJit(const std::vector<uint8_t>& executable){
  void *jitPtr = mmap(nullptr, 4096, PROT_READ | PROT_EXEC | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  std::memcpy(jitPtr, executable.data(), executable.size());
  return (void(*)())jitPtr;
}
#endif