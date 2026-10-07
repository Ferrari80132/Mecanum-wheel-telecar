/**
  ******************************************************************************
  * @file           : ps2_hal.h
  * @brief          : PS2手柄驱动头文件 (HAL库版本 for STM32F103)
  ******************************************************************************
  * 修改自XTARK原版驱动，适配STM32F103 HAL库
  *
  * PS2数据格式:
  *   BYTE   DATA   说明
  *   01     idle
  *   02     0x73   手柄工作模式
  *   03     0x5A   Bit0  Bit1  Bit2  Bit3  Bit4  Bit5  Bit6  Bit7
  *   04     data   SLCT  JOYR  JOYL  STRT   UP   RIGHT  DOWN   L
  *   05     data   L2     R2     L1    R1   /\     O     X    □
  *   06     data   右摇杆  0x00 = 左    0xff = 右
  *   07     data   右摇杆  0x00 = 上    0xff = 下
  *   08     data   左摇杆  0x00 = 左    0xff = 右
  *   09     data   左摇杆  0x00 = 上    0xff = 下
  *
  ******************************************************************************
  */

#ifndef __PS2_HAL_H
#define __PS2_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include <stdint.h>

/* PS2手柄引脚定义 (使用GPIOB) ----------------------------------------------- */
#define PS2_DI_GPIO_Port        GPIOB
#define PS2_DI_Pin             GPIO_PIN_12    // 数据输入引脚 (MISO)

#define PS2_CMD_GPIO_Port       GPIOB
#define PS2_CMD_Pin            GPIO_PIN_13    // 命令输出引脚 (MOSI)

#define PS2_CS_GPIO_Port        GPIOB
#define PS2_CS_Pin             GPIO_PIN_14    // 片选引脚

#define PS2_CLK_GPIO_Port       GPIOB
#define PS2_CLK_Pin            GPIO_PIN_15    // 时钟引脚

/* PS2手柄按键位定义 --------------------------------------------------------- */
// btn1 按键位
#define PS2_SELECT      (1 << 0)
#define PS2_L3          (1 << 1)  // 左摇杆按下
#define PS2_R3          (1 << 2)  // 右摇杆按下
#define PS2_START       (1 << 3)
#define PS2_UP          (1 << 4)
#define PS2_RIGHT       (1 << 5)
#define PS2_DOWN        (1 << 6)
#define PS2_LEFT        (1 << 7)

// btn2 按键位
#define PS2_L2          (1 << 0)
#define PS2_R2          (1 << 1)
#define PS2_L1          (1 << 2)
#define PS2_R1          (1 << 3)
#define PS2_TRIANGLE    (1 << 4)  // △
#define PS2_CIRCLE      (1 << 5)  // ○
#define PS2_CROSS       (1 << 6)  // ×
#define PS2_SQUARE      (1 << 7)  // □

/* PS2手柄数据结构体 --------------------------------------------------------- */
typedef struct
{
    uint8_t mode;        // 手柄工作模式 (0x73=模拟模式, 0x41=数字模式)

    uint8_t btn1;        // 按键组1: SELECT/L3/R3/START/UP/RIGHT/DOWN/LEFT

    uint8_t btn2;        // 按键组2: L2/R2/L1/R1/△/○/×/□

    uint8_t RJoy_LR;     // 右摇杆左右: 0x00=左  0x80=中  0xFF=右

    uint8_t RJoy_UD;     // 右摇杆上下: 0x00=上  0x80=中  0xFF=下

    uint8_t LJoy_LR;     // 左摇杆左右: 0x00=左  0x80=中  0xFF=右

    uint8_t LJoy_UD;     // 左摇杆上下: 0x00=上  0x80=中  0xFF=下

} PS2_TypeDef;

/* 函数声明 ------------------------------------------------------------------ */
void PS2_Init(void);
void PS2_ScanKey(PS2_TypeDef *ps2);
uint8_t PS2_CheckConnected(void);

#ifdef __cplusplus
}
#endif

#endif /* __PS2_HAL_H */
