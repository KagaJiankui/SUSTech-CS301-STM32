/**
 * @file hamming.c
 * @author [pachina(falkensakurai@outlook.com)](https://github.com/KagaJiankui)
 * @brief
 * @version 0.1
 * @date 2024-10-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "hamming.h"

#include "main.h"

/**
 * @brief: Hamming(7,4)编码
 *
 * 将4bit的message编码成7bit的Hamming码, bit8恒定为0
 *  message=[m0, m1, m2, m3]
 *  Hamming Encode=[p1, p2, m0, p3, m1, m2, m3]
 *
 * @param message 4bit的数据
 * @return 8bit的Hamming码
 */
uint8_t HammingEncode(uint8_t message) {
  uint8_t encoded = 0;
  // 截取计算校验位所需的bits
  // message=[m0, m1, m2, m3]
  //  temp_0=[ 0, m0, m1, m3]
  //  temp_1=[ 0, m0, m2, m3]
  //  temp_2=[ 0, m1, m2, m3]
  uint8_t par_temp0 = ((0x0C & message) >> 1) | (0x01 & message);
  uint8_t par_temp1 = ((0x08 & message) >> 1) | (0x03 & message);
  uint8_t par_temp2 = 0x07 & message;
  // LUT计算校验位
  SETBIT(encoded, 6, XOR_LUT(par_temp0));
  SETBIT(encoded, 5, XOR_LUT(par_temp1));
  SETBIT(encoded, 3, XOR_LUT(par_temp2));
  // 完成编码
  SETBIT(encoded, 4, 0x0C & message);
  encoded |= par_temp2;
  return encoded;
}

/**
 * @brief Performs a parity check on an 7-bit Hamming encoded value.
 *
 * This function calculates the parity bits needed to verify the integrity
 * of the Hamming encoded data. It extracts the relevant bits needed for
 * parity bit calculation and applies XOR logic to determine the parity.
 *
 * @param encoded The 7-bit encoded Hamming value to check.
 * @return The calculated parity value. This can be used to detect errors
 *         in the encoded data.
 */
uint8_t HammingParityCheck(uint8_t encoded) {
  uint8_t parity = 0;
  // 截取校验位所在的bits
  // encoded=[h0, h1, h2, h3, h4, h5, h6]
  //  temp_0=[h0,     h2,     h4,     h6]
  //  temp_1=[    h1, h2,         h5, h6]
  //  temp_2=[            h3, h4, h5, h6]
  uint8_t check_bit0 = ((0x40 & encoded) >> 3) | ((0x10 & encoded) >> 2) | ((0x04 & encoded) >> 1) | (0x01 & encoded);
  uint8_t check_bit1 = ((0x20 & encoded) >> 2) | ((0x10 & encoded) >> 2) | (0x03 & encoded);
  uint8_t check_bit2 = 0x0F & encoded;
  SETBIT(parity, 2, (((XOR_TT) & (1 << check_bit0)) != 0));
  SETBIT(parity, 1, (((XOR_TT) & (1 << check_bit1)) != 0));
  SETBIT(parity, 0, (((XOR_TT) & (1 << check_bit2)) != 0));
  return parity;
}

/**
 * @brief Corrects errors in a Hamming encoded message if any are detected.
 *
 * This function checks the parity of a given Hamming encoded message
 * and corrects errors by flipping the bit at the position indicated by
 * the parity value, if the parity is non-zero. After correction, it
 * extracts the raw message bits from the encoded message.
 *
 * @param hHammingMsg A pointer to the HammingMessage structure containing
 *                    the encoded message, parity bit information, and
 *                    other related data.
 */
void hammingFEC(HammingMessage* hHammingMsg) {
  uint8_t encoded_temp = hHammingMsg->encoded;
  if (hHammingMsg->parity != 0) {
    encoded_temp ^= (0x01 << hHammingMsg->parity);
  }
  hHammingMsg->message = GETRAW(encoded_temp);
}

/**
 * @brief Converts a uint8_t number to a binary string with a specified length. This function works by looping through the bits of the number from
most significant to least significant bit.
 *
 * For each bit, the function sets the corresponding character in the string to '0' or '1', and then shifts the number right to move to the next bit.
If the length of the string is less than 8 or greater than 1, the function will not modify the string.
 *
 * @param[in] num: The number to convert.
 * @param[in] len: The length of the binary string. Must be between 1 and 8.
 * @param[out] str: The binary string.
 */
void uintToBinString(uint8_t num, uint8_t pStrStart, uint8_t pStrEnd, uint8_t* str) {
  uint8_t len = pStrEnd - pStrStart;
  if (len < 8 && len > 0) {
    for (uint8_t i = 0; i < len; i++) {
      str[pStrEnd - i - 1] = (num & 1) + '0';
      num >>= 1;
    }
  } else {
    return;
  }
}
