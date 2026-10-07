/**
  ******************************************************************************
  * @file           : ps2_hal.c
  * @brief          : PS2手柄驱动实现文件 (HAL库版本 for STM32F103)
  ******************************************************************************
  * 修改自XTARK原版驱动，适配STM32F103 HAL库
  ******************************************************************************
  */

#include "ps2_hal.h"

/* 私有宏定义 ---------------------------------------------------------------- */
#define DI()        HAL_GPIO_ReadPin(PS2_DI_GPIO_Port, PS2_DI_Pin)
#define CMD_H()     HAL_GPIO_WritePin(PS2_CMD_GPIO_Port, PS2_CMD_Pin, GPIO_PIN_SET)
#define CMD_L()     HAL_GPIO_WritePin(PS2_CMD_GPIO_Port, PS2_CMD_Pin, GPIO_PIN_RESET)
#define CS_H()      HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET)
#define CS_L()      HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET)
#define CLK_H()     HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_SET)
#define CLK_L()     HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_RESET)

/* 私有变量 ------------------------------------------------------------------ */
// PS2命令序列：请求数据
const uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t PS2_data[9] = {0};  // 接收到的数据

/* 私有函数声明 -------------------------------------------------------------- */
static void PS2_Delay_us(uint32_t us);
static uint8_t PS2_ReadWriteByte(uint8_t data);

/**
  * @brief  微秒级延时函数
  * @param  us: 延时微秒数
  * @retval None
  */
static void PS2_Delay_us(uint32_t us)
{
    uint32_t delay = us * (SystemCoreClock / 8000000);
    while(delay--)
    {
        __NOP();
    }
}

/**
  * @brief  PS2数据读写函数 (模拟SPI)
  * @param  data: 要发送的数据
  * @retval 读取到的数据
  */
static uint8_t PS2_ReadWriteByte(uint8_t data)
{
    uint8_t ref;
    uint8_t res = 0;

    for(ref = 0x01; ref > 0x00; ref <<= 1)
    {
        // 发送数据位 (LSB first)
        if(ref&data)
            CMD_H();
        else
            CMD_L();

        CLK_L();
        PS2_Delay_us(16);

        if(DI())
            res |= ref;
        CLK_H();
        PS2_Delay_us(16);
    }
    return res;
}

/**
  * @brief  PS2手柄初始化
  * @param  None
  * @retval None
  */
void PS2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能GPIOB时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置DI引脚为输入上拉
    GPIO_InitStruct.Pin = PS2_DI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PS2_DI_GPIO_Port, &GPIO_InitStruct);

    // 配置CMD引脚为推挽输出
    GPIO_InitStruct.Pin = PS2_CMD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PS2_CMD_GPIO_Port, &GPIO_InitStruct);

    // 配置CS引脚为推挽输出
    GPIO_InitStruct.Pin = PS2_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PS2_CS_GPIO_Port, &GPIO_InitStruct);

    // 配置CLK引脚为推挽输出
    GPIO_InitStruct.Pin = PS2_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PS2_CLK_GPIO_Port, &GPIO_InitStruct);

    // 初始化引脚状态
    CMD_H();
    CLK_H();
    CS_H();
}

/**
  * @brief  PS2手柄扫描按键和摇杆
  * @param  ps2: PS2数据结构体指针
  * @retval None
  */
void PS2_ScanKey(PS2_TypeDef *ps2)
{
    uint8_t i;

    // 使能手柄 (CS拉低)
    CS_L();

    // 发送命令并接收数据
    for(i = 0; i < 9; i++)
    {
        PS2_data[i] = PS2_ReadWriteByte(PS2_cmnd[i]);
    }

    // 禁用手柄 (CS拉高)
    CS_H();

    // 解析数据
    ps2->mode = PS2_data[1];
    ps2->btn1 = ~PS2_data[3];  // 按键按下为0，取反后为1
    ps2->btn2 = ~PS2_data[4];
    ps2->RJoy_LR = PS2_data[5];
    ps2->RJoy_UD = PS2_data[6];
    ps2->LJoy_LR = PS2_data[7];
    ps2->LJoy_UD = PS2_data[8];
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
