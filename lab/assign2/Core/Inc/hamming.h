/**
 * @file hamming.h
 * @author [pachina(falkensakurai@outlook.com)](https://github.com/KagaJiankui)
 * @brief
 * @version 0.1
 * @date 2024-10-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "main.h"

/*Constant definition region*/
#define XOR_TT 0x9696
#define HAMMING_MASK3 0x07
#define HAMMING_MASK4 0x0F

typedef struct {
  uint8_t message;
  uint8_t encoded;
  uint8_t parity;
  uint8_t pBit;
} HammingMessage;
/* End constant definition region*/

// 定义SETBIT宏，用于设置num的第bit位为bool_val (1或0)
// bit遵循小端序, bit=0时操作末位
#define SETBIT(num, bit, bool_val) ((bool_val) ? ((num) |= (1 << (bit))) : ((num) &= ~(1 << (bit))))

// 定义GETBIT宏，用于获取num的第bit位的值 (0或1)
// bit遵循小端序, bit=0时操作末位
#define GETBIT(num, bit) (((num) & (1 << (bit))) != 0)

// 定义XOR_LUT宏，用于查找表计算多元异或运算 xor(X[4]) 的值
#define XOR_LUT(value) GETBIT(XOR_TT, value)

// 定义CORRECT_ERROR宏，使用校验位计算结果校正错误
#define CORRECT_ERROR(encoded, error) ((error) > 0 ? ((encoded) ^= (1 << (error))) : (encoded))

// 定义GETRAW宏，用于获取未经校验的数据位
#define GETRAW(encoded) (((encoded) & 0x10) >> 1) | ((encoded) & HAMMING_MASK3)

/*Exported Functions*/

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
uint8_t HammingEncode(uint8_t message);

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
uint8_t HammingParityCheck(uint8_t encoded);

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
void hammingFEC(HammingMessage* hHammingMsg);

/**
 * @brief Converts a uint8_t number to a binary string with a specified length. This function works by looping through the bits of the number from
most significant to least significant bit.
 *
 * For each bit, the function sets the corresponding character in the string to '0' or '1', and then shifts the number right to move to the next
bit. If the length of the string is less than 8 or greater than 1, the function will not modify the string.
 *
 * @param[in] num: The number to convert.
 * @param[in] len: The length of the binary string. Must be between 1 and 8.
 * @param[out] str: The binary string.
 */
void uintToBinString(uint8_t num, uint8_t pStrStart, uint8_t pStrEnd, uint8_t* str);
