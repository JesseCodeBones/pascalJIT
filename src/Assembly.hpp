#ifndef __pascal_jit_assembly__
#define __pascal_jit_assembly__
#include <vector>
#include <cstdint>
#include <iostream>


static void addUint32_t(std::vector<uint8_t>& executable, const uint32_t value){
  executable.push_back(value & 0xff);
  executable.push_back((value >> (sizeof(uint8_t) * 8)) & 0xff);
  executable.push_back((value >> ((2*sizeof(uint8_t)) * 8)) & 0xff);
  executable.push_back((value >> ((3*sizeof(uint8_t)) * 8)) & 0xff);
}

static std::vector<uint8_t> callRegister(uint32_t register1){
  std::vector<uint8_t> asms;
  uint32_t call_asm = 0b1101011000111111000000 << 10;
  call_asm |= (register1 << 5); // register << 5
  addUint32_t(asms, call_asm);
  return asms;
}

static std::vector<uint8_t> insertPtrToRegister(const uint8_t registerIndex, void const*const ptr) {

  const uintptr_t value = (uintptr_t)ptr;
  std::vector<uint8_t> asms;

  uint32_t mov_x_2 = 0b10100101 << 23;
  mov_x_2 |= ((value & 0xffff) << 5);
  mov_x_2 |= registerIndex;
  uint32_t mov_x_4 = 0b111100101 << 23;
  mov_x_4 |= (((value >> 16) & 0xffff) << 5);
  mov_x_4 |= registerIndex;
  mov_x_4 |= (1 << 21);
  uint32_t mov_x_6 = 0b111100101 << 23;
  mov_x_6 |= (((value >> 32) & 0xffff) << 5);
  mov_x_6 |= registerIndex;
  mov_x_6 |= (2 << 21);
  uint32_t mov_x_8 = 0b111100101 << 23;
  mov_x_8 |= (((value >> 48) & 0xffff) << 5);
  mov_x_8 |= registerIndex;
  mov_x_8 |= (3 << 21);
  addUint32_t(asms, mov_x_2);
  addUint32_t(asms, mov_x_4);
  addUint32_t(asms, mov_x_6);
  addUint32_t(asms, mov_x_8);
  return asms;
}

static void addAssemblyToExecutable(std::vector<uint8_t> & executable, std::vector<uint8_t> assembly) {
  for(auto code : assembly) {
    executable.push_back(code);
  }
}

static std::vector<uint8_t> ret(){
  std::vector<uint8_t> assemblies;
  uint32_t r = 0xD65F0000 | (30 << 5);
  addUint32_t(assemblies, r);
  return assemblies;
}


static std::vector<uint8_t> storeX29X30(){

  std::vector<uint8_t> assemblies;
  uint32_t stp_x29_x30 = 0b101010011 << 23;
  stp_x29_x30 |= (0x2211 << 5);
  stp_x29_x30 |= (((-16 / 8) & 0b1111111) << 15); // IMM7
  stp_x29_x30 |= (30 << 10); // RT2
  stp_x29_x30 |= (31 << 5); // RN sp = X31
  stp_x29_x30 |= (29); // RT
  addUint32_t(assemblies, stp_x29_x30);
  return assemblies;
}

static std::vector<uint8_t> loadX29X30(){

  std::vector<uint8_t> assemblies;
  uint32_t LDP_X29_X30 = 0b1010100011 << 22;
  LDP_X29_X30 |= (((16 / 8) & 0b1111111) << 15); // IMM7
  LDP_X29_X30 |= (30 << 10); // RT2
  LDP_X29_X30 |= (31 << 5); // RN sp = X31
  LDP_X29_X30 |= (29); // RT
  addUint32_t(assemblies, LDP_X29_X30);
  return assemblies;
}


#endif