#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "ft5206.h"
#include "gt9147.h"
#include "ott2001a.h"
#include "stdio.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
// 只学习使矫ｏ拷未桑魏酵?
// ALIENTEK STM32
// (支ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A/GT9147/FT5206)
// 原@ALIENTEK
// 坛:www.openedv.com
// :2015/1/15
// 芥本V1.0
// 权叫ｏ拷鼐?
// Copyright(C) 涌萍薰剿? 2009-2019
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#define TP_PRES_DOWN 0x80  //
#define TP_CATH_PRES 0x40  // 叫帮拷
#define CT_MAX_TOUCH 5     // 支街的碉拷,教讹拷为5

//
typedef struct {
  u8 (*init)(void);      // 鍒濆?嬪寲鐢甸樆灞?
  u8 (*scan)(u8);        // 扫借触.0,幕扫;1,;
  void (*adjust)(void);  // 校准
  u16 x[CT_MAX_TOUCH];   // 前
  u16 y[CT_MAX_TOUCH];   // ?5,x[0],y[0]:剿达拷扫时,,
                         // x[4],y[4]芥储一轿帮拷时.
  u8 sta;                // 绞碉拷状态
                         // b7:1/缴匡拷0;
  // b6:0,没叫帮拷;1,叫帮拷.
  // b5:
  // b4~b0:捷达拷铰的碉拷(0,示未,1示)
  /////////////////////校准(要校准)//////////////////////
  float xfac;  // 鏂滅巼
  float yfac;
  short xoff;  // 鍋忕Щ
  short yoff;
  // 侥诧拷,全竭碉拷时要矫碉拷.
  // b0:0,(绞猴拷为X,为Y絋P)
  //    1,(绞猴拷为Y,为X絋P)
  // b1~6:.
  // b7:0,
  //    1,
  u8 touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;  // touch.c芥定

// /芯片
#define PEN  		PCin(1)  	//PC1  INT
#define DOUT 		PCin(2)   	//PC2  MISO
#define TDIN 		PCout(3)  	//PC3  MOSI
#define TCLK 		PCout(0)  	//PC0  SCLK
#define TCS PCout(13)                   // PC13 CS

//
void TP_Write_Byte(u8 num);                                                                      // 叫酒?写一
u16 TP_Read_AD(u8 CMD);                                                                          // 取AD转值
u16 TP_Read_XOY(u8 xy);                                                                          // 剿诧拷饺?(X/Y)
u8 TP_Read_XY(u16 *x, u16 *y);                                                                   // 双饺?(X+Y)
u8 TP_Read_XY2(u16 *x, u16 *y);                                                                  // 强剿诧拷双饺?
void TP_Drow_Touch_Point(u16 x, u16 y, u16 color);                                               // 一校准
void TP_Draw_Big_Point(u16 x, u16 y, u16 color);                                                 // 一?
void TP_Save_Adjdata(void);                                                                      // 校准
u8 TP_Get_Adjdata(void);                                                                         // 取校准
void TP_Adjust(void);                                                                            // 校准
void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac);  // 示校准息
/// 矫猴拷
u8 TP_Scan(u8 tp);  // 扫
u8 TP_Init(void);   // 始

#endif
