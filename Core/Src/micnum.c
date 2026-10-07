#include "micnum.h"

void Car_Rotate_Left(void)
{
    // 左侧轮子后退，右侧轮子前进
    HAL_GPIO_WritePin(LU_0_GPIO_Port, LU_0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LU_1_GPIO_Port, LU_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RU_0_GPIO_Port, RU_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RU_1_GPIO_Port, RU_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LD_0_GPIO_Port, LD_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_1_GPIO_Port, LD_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RD_0_GPIO_Port, RD_0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RD_1_GPIO_Port, RD_1_Pin, GPIO_PIN_RESET);
}

void Car_Rotate_Right(void)
{
    // 左侧轮子前进，右侧轮子后退
    HAL_GPIO_WritePin(LU_0_GPIO_Port, LU_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LU_1_GPIO_Port, LU_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RU_0_GPIO_Port, RU_0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RU_1_GPIO_Port, RU_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_0_GPIO_Port, LD_0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LD_1_GPIO_Port, LD_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RD_0_GPIO_Port, RD_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RD_1_GPIO_Port, RD_1_Pin, GPIO_PIN_SET);
}

void Car_Stop(void)
{
    // 所有电机停止
    HAL_GPIO_WritePin(LU_0_GPIO_Port, LU_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LU_1_GPIO_Port, LU_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RU_0_GPIO_Port, RU_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RU_1_GPIO_Port, RU_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_0_GPIO_Port, LD_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD_1_GPIO_Port, LD_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RD_0_GPIO_Port, RD_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RD_1_GPIO_Port, RD_1_Pin, GPIO_PIN_RESET);
}

// 设置单个电机的方向和速度
// motor: 1=LU, 2=RU, 3=LD, 4=RD
// speed: -100到100，负数表示反转
extern TIM_HandleTypeDef htim1;

void Set_Motor_Speed(uint8_t motor, int speed)
{
    uint32_t channel;
    GPIO_TypeDef *port_0, *port_1;
    uint16_t pin_0, pin_1;

    // 选择对应的通道和引脚
    switch(motor)
    {
        case 1: // LU
            channel = TIM_CHANNEL_1;
            port_0 = LU_0_GPIO_Port;
            pin_0 = LU_0_Pin;
            port_1 = LU_1_GPIO_Port;
            pin_1 = LU_1_Pin;
            break;
        case 2: // RU
            channel = TIM_CHANNEL_2;
            port_0 = RU_0_GPIO_Port;
            pin_0 = RU_0_Pin;
            port_1 = RU_1_GPIO_Port;
            pin_1 = RU_1_Pin;
            break;
        case 3: // LD
            channel = TIM_CHANNEL_3;
            port_0 = LD_0_GPIO_Port;
            pin_0 = LD_0_Pin;
            port_1 = LD_1_GPIO_Port;
            pin_1 = LD_1_Pin;
            break;
        case 4: // RD
            channel = TIM_CHANNEL_4;
            port_0 = RD_0_GPIO_Port;
            pin_0 = RD_0_Pin;
            port_1 = RD_1_GPIO_Port;
            pin_1 = RD_1_Pin;
            break;
        default:
            return;
    }

    // 设置方向
    if(speed > 0)
    {
        HAL_GPIO_WritePin(port_0, pin_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port_1, pin_1, GPIO_PIN_SET);
    }
    else if(speed < 0)
    {
        HAL_GPIO_WritePin(port_0, pin_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(port_1, pin_1, GPIO_PIN_RESET);
        speed = -speed; // 转为正数用于PWM
    }
    else
    {
        HAL_GPIO_WritePin(port_0, pin_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port_1, pin_1, GPIO_PIN_RESET);
    }

    // 限制速度范围
    if(speed > 100) speed = 100;

    // 设置PWM占空比
    __HAL_TIM_SET_COMPARE(&htim1, channel, speed);
}
