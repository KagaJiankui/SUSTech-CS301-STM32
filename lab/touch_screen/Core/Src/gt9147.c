#include "gt9147.h"
#include "touch.h"
#include "ctiic.h"
//#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////
// 锟斤拷锟斤拷锟斤拷只锟斤拷学习使锟矫ｏ拷未锟斤拷锟斤拷锟斤拷锟斤拷桑锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷魏锟斤拷锟酵�
// ALIENTEK STM32锟斤拷锟斤拷锟斤拷
// 4.3锟斤拷锟斤拷荽锟斤拷锟斤拷锟�-GT9147 锟斤拷锟斤拷锟斤拷锟斤拷
// 锟斤拷锟斤拷原锟斤拷@ALIENTEK
// 锟斤拷锟斤拷锟斤拷坛:www.openedv.com
// 锟斤拷锟斤拷锟斤拷锟斤拷:2015/1/15
// 锟芥本锟斤拷V1.1
// 锟斤拷权锟斤拷锟叫ｏ拷锟斤拷锟斤拷鼐锟斤拷锟�
// Copyright(C) 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷涌萍锟斤拷锟斤拷薰锟剿� 2014-2024
// All rights reserved
//*******************************************************************************
// 锟睫革拷锟斤拷息
// V1.1 20150320
// 锟睫革拷GT9147_Scan锟斤拷锟斤拷,锟斤拷佣苑欠锟斤拷锟斤拷莸拇锟斤拷锟�,锟斤拷止锟角凤拷锟斤拷锟捷革拷锟斤拷
//////////////////////////////////////////////////////////////////////////////////

// GT9147锟斤拷锟矫诧拷锟斤拷锟斤拷
// 锟斤拷一锟斤拷锟街斤拷为锟芥本锟斤拷(0X60),锟斤拷锟诫保证锟铰的版本锟脚达拷锟节碉拷锟斤拷GT9147锟节诧拷
// flash原锟叫版本锟斤拷,锟脚伙拷锟斤拷锟斤拷锟斤拷锟�.
const u8 GT9147_CFG_TBL[] = {
    0X60, 0XE0, 0X01, 0X20, 0X03, 0X05, 0X35, 0X00, 0X02, 0X08, 0X1E, 0X08, 0X50, 0X3C, 0X0F, 0X05, 0X00, 0X00, 0XFF, 0X67, 0X50, 0X00, 0X00,
    0X18, 0X1A, 0X1E, 0X14, 0X89, 0X28, 0X0A, 0X30, 0X2E, 0XBB, 0X0A, 0X03, 0X00, 0X00, 0X02, 0X33, 0X1D, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X32, 0X00, 0X00, 0X2A, 0X1C, 0X5A, 0X94, 0XC5, 0X02, 0X07, 0X00, 0X00, 0X00, 0XB5, 0X1F, 0X00, 0X90, 0X28, 0X00, 0X77, 0X32, 0X00,
    0X62, 0X3F, 0X00, 0X52, 0X50, 0X00, 0X52, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X0F, 0X0F, 0X03, 0X06, 0X10, 0X42, 0XF8, 0X0F, 0X14, 0X00, 0X00, 0X00, 0X00, 0X1A, 0X18, 0X16,
    0X14, 0X12, 0X10, 0X0E, 0X0C, 0X0A, 0X08, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X29, 0X28, 0X24, 0X22, 0X20, 0X1F, 0X1E, 0X1D, 0X0E, 0X0C, 0X0A, 0X08, 0X06, 0X05, 0X04, 0X02, 0X00, 0XFF, 0X00,
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
};
// 锟斤拷锟斤拷GT9147锟斤拷锟矫诧拷锟斤拷
// mode:0,锟斤拷锟斤拷锟斤拷锟斤拷锟芥到flash
//      1,锟斤拷锟斤拷锟斤拷锟芥到flash
u8 GT9147_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
        buf[1] = mode;  // 锟角凤拷写锟诫到GT9147 FLASH?  锟斤拷锟角凤拷锟斤拷绫ｏ拷锟�
        for (i = 0; i < sizeof(GT9147_CFG_TBL); i++) buf[0] += GT9147_CFG_TBL[i];  // 锟斤拷锟斤拷校锟斤拷锟�
        buf[0] = (~buf[0]) + 1;
        GT9147_WR_Reg(GT_CFGS_REG, (u8 *)GT9147_CFG_TBL, sizeof(GT9147_CFG_TBL));  // 锟斤拷锟酵寄达拷锟斤拷锟斤拷锟斤拷
        GT9147_WR_Reg(GT_CHECK_REG, buf, 2);  // 写锟斤拷校锟斤拷锟�,锟斤拷锟斤拷锟矫革拷锟铰憋拷锟�
        return 0;
}
// 锟斤拷GT9147写锟斤拷一锟斤拷锟斤拷锟斤拷
// reg:锟斤拷始锟侥达拷锟斤拷锟斤拷址
// buf:锟斤拷锟捷伙拷锟斤拷锟斤拷锟斤拷
// len:写锟斤拷锟捷筹拷锟斤拷
// 锟斤拷锟斤拷值:0,锟缴癸拷;1,失锟斤拷.
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
        CT_IIC_Start();
        CT_IIC_Send_Byte(GT_CMD_WR);  // 锟斤拷锟斤拷写锟斤拷锟斤拷
        CT_IIC_Wait_Ack();
        CT_IIC_Send_Byte(reg >> 8);  // 锟斤拷锟酵革拷8位锟斤拷址
        CT_IIC_Wait_Ack();
        CT_IIC_Send_Byte(reg & 0XFF);  // 锟斤拷锟酵碉拷8位锟斤拷址
        CT_IIC_Wait_Ack();
        for (i = 0; i < len; i++) {
          CT_IIC_Send_Byte(buf[i]);  // 锟斤拷锟斤拷锟斤拷
          ret = CT_IIC_Wait_Ack();
          if (ret) break;
        }
        CT_IIC_Stop();  // 锟斤拷锟斤拷一锟斤拷停止锟斤拷锟斤拷
        return ret;
}
// 锟斤拷GT9147锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷
// reg:锟斤拷始锟侥达拷锟斤拷锟斤拷址
// buf:锟斤拷锟捷伙拷锟斤拷锟斤拷锟斤拷
// len:锟斤拷锟斤拷锟捷筹拷锟斤拷
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
  u8 i;
  CT_IIC_Start();
  CT_IIC_Send_Byte(GT_CMD_WR);  // 锟斤拷锟斤拷写锟斤拷锟斤拷
  CT_IIC_Wait_Ack();
  CT_IIC_Send_Byte(reg >> 8);  // 锟斤拷锟酵革拷8位锟斤拷址
  CT_IIC_Wait_Ack();
  CT_IIC_Send_Byte(reg & 0XFF);  // 锟斤拷锟酵碉拷8位锟斤拷址
  CT_IIC_Wait_Ack();
  CT_IIC_Start();
  CT_IIC_Send_Byte(GT_CMD_RD);  // 锟斤拷锟酵讹拷锟斤拷锟斤拷
  CT_IIC_Wait_Ack();
  for (i = 0; i < len; i++) {
    buf[i] = CT_IIC_Read_Byte(i == (len - 1) ? 0 : 1);  // 锟斤拷锟斤拷锟斤拷
  }
        CT_IIC_Stop();  // 锟斤拷锟斤拷一锟斤拷停止锟斤拷锟斤拷
}
// 锟斤拷始锟斤拷GT9147锟斤拷锟斤拷锟斤拷
// 锟斤拷锟斤拷值:0,锟斤拷始锟斤拷锟缴癸拷;1,锟斤拷始锟斤拷失锟斤拷
u8 GT9147_Init(void)
{
  u8 temp[5];

  GPIO_InitTypeDef GPIO_Initure;

  __HAL_RCC_GPIOC_CLK_ENABLE();  // 锟斤拷锟斤拷GPIOC时锟斤拷

  GPIO_Initure.Pin = GPIO_PIN_13;             // PC13
  GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;    // 锟斤拷锟斤拷锟斤拷锟�
  GPIO_Initure.Pull = GPIO_PULLUP;            // 锟斤拷锟斤拷
  GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;  // 锟斤拷锟斤拷
  HAL_GPIO_Init(GPIOC, &GPIO_Initure);

  GPIO_Initure.Pin = GPIO_PIN_1;        // PC1
  GPIO_Initure.Mode = GPIO_MODE_INPUT;  // 锟斤拷锟斤拷
  HAL_GPIO_Init(GPIOC, &GPIO_Initure);

  CT_IIC_Init();  // 锟斤拷始锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷I2C锟斤拷锟斤拷
  GT_RST = 0;     // 锟斤拷位
  delay_ms(10);
  GT_RST = 1;  // 锟酵放革拷位
  delay_ms(10);

  GPIO_Initure.Pin = GPIO_PIN_1;        // PC1
  GPIO_Initure.Pull = GPIO_PULLDOWN;    // 锟斤拷锟斤拷锟斤拷锟斤拷
  GPIO_Initure.Mode = GPIO_MODE_INPUT;  // 锟斤拷锟斤拷锟斤拷锟�
  HAL_GPIO_Init(GPIOC, &GPIO_Initure);

  delay_ms(100);
  GT9147_RD_Reg(GT_PID_REG, temp, 4);  // 锟斤拷取锟斤拷品ID
  temp[4] = 0;
  printf("CTP ID:%s\r\n", temp);          // 锟斤拷印ID
  if (strcmp((char *)temp, "9147") == 0)  // ID==9147
  {
    temp[0] = 0X02;
    GT9147_WR_Reg(GT_CTRL_REG, temp, 1);  // 锟斤拷位GT9147
    GT9147_RD_Reg(GT_CFGS_REG, temp, 1);  // 锟斤拷取GT_CFGS_REG锟侥达拷锟斤拷
    if (temp[0] < 0X60)                   // 默锟较版本锟饺较碉拷,锟斤拷要锟斤拷锟斤拷flash锟斤拷锟斤拷
    {
      printf("Default Ver:%d\r\n", temp[0]);
      GT9147_Send_Cfg(1);  // 锟斤拷锟铰诧拷锟斤拷锟斤拷锟斤拷锟斤拷
    }
    delay_ms(10);
    temp[0] = 0X00;
    GT9147_WR_Reg(GT_CTRL_REG, temp, 1);  // 锟斤拷锟斤拷锟斤拷位
    return 0;
  }
        return 0;
}
const u16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
// 扫锟借触锟斤拷锟斤拷(锟斤拷锟矫诧拷询锟斤拷式)
// mode:0,锟斤拷锟斤拷扫锟斤拷.
// 锟斤拷锟斤拷值:锟斤拷前锟斤拷锟斤拷状态.
// 0,锟斤拷锟斤拷锟睫达拷锟斤拷;1,锟斤拷锟斤拷锟叫达拷锟斤拷
u8 GT9147_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
        static u8 t = 0;  // 锟斤拷锟狡诧拷询锟斤拷锟�,锟接讹拷锟斤拷锟斤拷CPU占锟斤拷锟斤拷
        t++;
        if ((t % 10) == 0 || t < 10)  // 锟斤拷锟斤拷时,每锟斤拷锟斤拷10锟斤拷CTP_Scan锟斤拷锟斤拷锟脚硷拷锟�1锟斤拷,锟接讹拷锟斤拷省CPU使锟斤拷锟斤拷
        {
          GT9147_RD_Reg(GT_GSTID_REG, &mode, 1);  // 锟斤拷取锟斤拷锟斤拷锟斤拷锟阶刺�
          if (mode & 0X80 && ((mode & 0XF) < 6)) {
            temp = 0;
            GT9147_WR_Reg(GT_GSTID_REG, &temp, 1);  // 锟斤拷锟街�
          }
          if ((mode & 0XF) && ((mode & 0XF) < 6)) {
            temp = 0XFF << (mode & 0XF);  // 锟斤拷锟斤拷母锟斤拷锟阶�锟斤拷为1锟斤拷位锟斤拷,匹锟斤拷tp_dev.sta锟斤拷锟斤拷
            tempsta = tp_dev.sta;  // 锟斤拷锟芥当前锟斤拷tp_dev.sta值
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            tp_dev.x[4] = tp_dev.x[0];  // 锟斤拷锟芥触锟斤拷0锟斤拷锟斤拷锟斤拷
            tp_dev.y[4] = tp_dev.y[0];
            for (i = 0; i < 5; i++) {
              if (tp_dev.sta & (1 << i))  // 锟斤拷锟斤拷锟斤拷效?
              {
                GT9147_RD_Reg(GT9147_TPX_TBL[i], buf, 4);  // 锟斤拷取XY锟斤拷锟斤拷值
                if (tp_dev.touchtype & 0X01)               // 锟斤拷锟斤拷
                {
                  tp_dev.y[i] = ((u16)buf[1] << 8) + buf[0];
                  tp_dev.x[i] = 800 - (((u16)buf[3] << 8) + buf[2]);
                } else {
                  tp_dev.x[i] = ((u16)buf[1] << 8) + buf[0];
                  tp_dev.y[i] = ((u16)buf[3] << 8) + buf[2];
                }
                // printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
              }
            }
            res = 1;
            if (tp_dev.x[0] > lcddev.width || tp_dev.y[0] > lcddev.height)  // 锟角凤拷锟斤拷锟斤拷(锟斤拷锟疥超锟斤拷锟斤拷)
            {
              if ((mode & 0XF) >
                  1)  // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷,锟津复第讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷莸锟斤拷锟揭伙拷锟斤拷锟斤拷锟�.
              {
                tp_dev.x[0] = tp_dev.x[1];
                tp_dev.y[0] = tp_dev.y[1];
                t = 0;  // 锟斤拷锟斤拷一锟斤拷,锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷10锟斤拷,锟接讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
              } else    // 锟角凤拷锟斤拷锟斤拷,锟斤拷锟斤拷源舜锟斤拷锟斤拷锟�(锟斤拷原原锟斤拷锟斤拷)
              {
                tp_dev.x[0] = tp_dev.x[4];
                tp_dev.y[0] = tp_dev.y[4];
                mode = 0X80;
                tp_dev.sta = tempsta;  // 锟街革拷tp_dev.sta
              }
            } else
              t = 0;  // 锟斤拷锟斤拷一锟斤拷,锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷10锟斤拷,锟接讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
          }
        }
        if ((mode & 0X8F) == 0X80)  // 锟睫达拷锟斤拷锟姐按锟斤拷
        {
          if (tp_dev.sta & TP_PRES_DOWN)  // 之前锟角憋拷锟斤拷锟铰碉拷
          {
            tp_dev.sta &= ~(1 << 7);  // 锟斤拷前锟斤拷锟斤拷煽锟�
          } else                      // 之前锟斤拷没锟叫憋拷锟斤拷锟斤拷
          {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE0;  // 锟斤拷锟斤拷锟斤拷锟叫э拷锟斤拷
          }
        }
        if (t > 240) t = 10;  // 锟斤拷锟铰达拷10锟斤拷始锟斤拷锟斤拷
        return res;
}
