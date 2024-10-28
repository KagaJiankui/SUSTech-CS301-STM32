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

/*Constant definition region*/
#define HAMMING_BASE 0x9696
#define HAMMING_MASK3 0x07
#define HAMMING_MASK4 0x0F
/* End constant definition region*/

// 定义SETBIT宏，用于设置num的第bit位为bool_val (1或0)
#define SETBIT(num, bit, bool_val) ((bool_val) ? ((num) |= (1 << (bit))) : ((num) &= ~(1 << (bit))))

// 定义GETBIT宏，用于获取num的第bit位的值 (0或1)
#define GETBIT(num, bit) (((num) >> (bit)) & 1)

// 定义XOR_LUT宏，用于查找表计算多元异或运算 xor(X[4]) 的值
#define XOR_LUT(value) GETBIT(HAMMING_BASE, value)
