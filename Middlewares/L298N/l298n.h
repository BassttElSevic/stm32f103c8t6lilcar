/**
  ******************************************************************************
  * @file    l298n.h
  * @brief   L298N motor driver interface
  * @author  Basstt
  ******************************************************************************
  */

#ifndef __L298N_H
#define __L298N_H

#include "stm32f1xx_hal.h"

/* Motor definitions */
typedef enum {
    MOTOR_FL = 0,  /* Front Left */
    MOTOR_FR = 1,  /* Front Right */
    MOTOR_RL = 2,  /* Rear Left */
    MOTOR_RR = 3,  /* Rear Right */
    MOTOR_COUNT = 4
} Motor_t;

/* Motor direction */
typedef enum {
    DIR_STOP = 0,
    DIR_FORWARD = 1,
    DIR_BACKWARD = 2
} MotorDir_t;

/* Motor control structure */
typedef struct {
    GPIO_TypeDef* IN1_Port;
    uint16_t IN1_Pin;
    GPIO_TypeDef* IN2_Port;
    uint16_t IN2_Pin;
    TIM_HandleTypeDef* htim;
    uint32_t TIM_Channel;
} MotorControl_t;

/* Function prototypes */
void L298N_Init(void);
void L298N_SetSpeed(Motor_t motor, uint16_t speed);
void L298N_SetDirection(Motor_t motor, MotorDir_t direction);
void L298N_MoveForward(uint16_t speed);
void L298N_MoveBackward(uint16_t speed);
void L298N_TurnLeft(uint16_t speed);
void L298N_TurnRight(uint16_t speed);
void L298N_Stop(void);

#endif /* __L298N_H */
