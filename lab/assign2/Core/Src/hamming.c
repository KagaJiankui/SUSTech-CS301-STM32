#include "main.h"
#include "hamming.h"

uint8_t HammingEncode(uint8_t message) {
  uint8_t encoded = 0;
  // 计算校验位
  uint8_t par_temp_0 = ((0x0C & message) >> 1) | (0x01 & message);
  uint8_t par_temp_1 = ((0x08 & message) >> 1) | (0x03 & message);
}