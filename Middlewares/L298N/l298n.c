/**
  ******************************************************************************
  * @file    l298n.c
  * @brief   L298N motor driver implementation
  * @author  Basstt
  ******************************************************************************
  */

#include "l298n.h"

/* External timer handles (defined in main.c) */
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/* Motor control array */
static MotorControl_t motors[MOTOR_COUNT];

/**
  * @brief  Initialize L298N motor drivers
  * @note   Motor connections:
  *         L298N #1: Motor FL (PA0, PA1, TIM2_CH1) and Motor FR (PA2, PA3, TIM2_CH2)
  *         L298N #2: Motor RL (PB0, PB1, TIM3_CH3) and Motor RR (PB6, PB7, TIM3_CH4)
  */
void L298N_Init(void)
{
    /* Motor FL - Front Left */
    motors[MOTOR_FL].IN1_Port = GPIOA;
    motors[MOTOR_FL].IN1_Pin = GPIO_PIN_0;
    motors[MOTOR_FL].IN2_Port = GPIOA;
    motors[MOTOR_FL].IN2_Pin = GPIO_PIN_1;
    motors[MOTOR_FL].htim = &htim2;
    motors[MOTOR_FL].TIM_Channel = TIM_CHANNEL_1;
    
    /* Motor FR - Front Right */
    motors[MOTOR_FR].IN1_Port = GPIOA;
    motors[MOTOR_FR].IN1_Pin = GPIO_PIN_2;
    motors[MOTOR_FR].IN2_Port = GPIOA;
    motors[MOTOR_FR].IN2_Pin = GPIO_PIN_3;
    motors[MOTOR_FR].htim = &htim2;
    motors[MOTOR_FR].TIM_Channel = TIM_CHANNEL_2;
    
    /* Motor RL - Rear Left */
    motors[MOTOR_RL].IN1_Port = GPIOB;
    motors[MOTOR_RL].IN1_Pin = GPIO_PIN_0;
    motors[MOTOR_RL].IN2_Port = GPIOB;
    motors[MOTOR_RL].IN2_Pin = GPIO_PIN_1;
    motors[MOTOR_RL].htim = &htim3;
    motors[MOTOR_RL].TIM_Channel = TIM_CHANNEL_3;
    
    /* Motor RR - Rear Right */
    motors[MOTOR_RR].IN1_Port = GPIOB;
    motors[MOTOR_RR].IN1_Pin = GPIO_PIN_6;
    motors[MOTOR_RR].IN2_Port = GPIOB;
    motors[MOTOR_RR].IN2_Pin = GPIO_PIN_7;
    motors[MOTOR_RR].htim = &htim3;
    motors[MOTOR_RR].TIM_Channel = TIM_CHANNEL_4;
    
    /* Stop all motors initially */
    L298N_Stop();
}

/**
  * @brief  Set motor speed (PWM duty cycle)
  * @param  motor: Motor identifier
  * @param  speed: Speed value (0-1000)
  */
void L298N_SetSpeed(Motor_t motor, uint16_t speed)
{
    if (motor >= MOTOR_COUNT) return;
    
    /* Limit speed to 1000 (100% duty cycle) */
    if (speed > 1000) speed = 1000;
    
    /* Set PWM duty cycle */
    __HAL_TIM_SET_COMPARE(motors[motor].htim, motors[motor].TIM_Channel, speed);
}

/**
  * @brief  Set motor direction
  * @param  motor: Motor identifier
  * @param  direction: Motor direction (STOP, FORWARD, BACKWARD)
  */
void L298N_SetDirection(Motor_t motor, MotorDir_t direction)
{
    if (motor >= MOTOR_COUNT) return;
    
    switch(direction) {
        case DIR_FORWARD:
            HAL_GPIO_WritePin(motors[motor].IN1_Port, motors[motor].IN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(motors[motor].IN2_Port, motors[motor].IN2_Pin, GPIO_PIN_RESET);
            break;
        case DIR_BACKWARD:
            HAL_GPIO_WritePin(motors[motor].IN1_Port, motors[motor].IN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(motors[motor].IN2_Port, motors[motor].IN2_Pin, GPIO_PIN_SET);
            break;
        case DIR_STOP:
        default:
            HAL_GPIO_WritePin(motors[motor].IN1_Port, motors[motor].IN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(motors[motor].IN2_Port, motors[motor].IN2_Pin, GPIO_PIN_RESET);
            break;
    }
}

/**
  * @brief  Move car forward
  * @param  speed: Speed value (0-1000)
  */
void L298N_MoveForward(uint16_t speed)
{
    for (int i = 0; i < MOTOR_COUNT; i++) {
        L298N_SetDirection((Motor_t)i, DIR_FORWARD);
        L298N_SetSpeed((Motor_t)i, speed);
    }
}

/**
  * @brief  Move car backward
  * @param  speed: Speed value (0-1000)
  */
void L298N_MoveBackward(uint16_t speed)
{
    for (int i = 0; i < MOTOR_COUNT; i++) {
        L298N_SetDirection((Motor_t)i, DIR_BACKWARD);
        L298N_SetSpeed((Motor_t)i, speed);
    }
}

/**
  * @brief  Turn car left
  * @param  speed: Speed value (0-1000)
  */
void L298N_TurnLeft(uint16_t speed)
{
    /* Left motors backward, right motors forward */
    L298N_SetDirection(MOTOR_FL, DIR_BACKWARD);
    L298N_SetSpeed(MOTOR_FL, speed);
    L298N_SetDirection(MOTOR_RL, DIR_BACKWARD);
    L298N_SetSpeed(MOTOR_RL, speed);
    
    L298N_SetDirection(MOTOR_FR, DIR_FORWARD);
    L298N_SetSpeed(MOTOR_FR, speed);
    L298N_SetDirection(MOTOR_RR, DIR_FORWARD);
    L298N_SetSpeed(MOTOR_RR, speed);
}

/**
  * @brief  Turn car right
  * @param  speed: Speed value (0-1000)
  */
void L298N_TurnRight(uint16_t speed)
{
    /* Right motors backward, left motors forward */
    L298N_SetDirection(MOTOR_FR, DIR_BACKWARD);
    L298N_SetSpeed(MOTOR_FR, speed);
    L298N_SetDirection(MOTOR_RR, DIR_BACKWARD);
    L298N_SetSpeed(MOTOR_RR, speed);
    
    L298N_SetDirection(MOTOR_FL, DIR_FORWARD);
    L298N_SetSpeed(MOTOR_FL, speed);
    L298N_SetDirection(MOTOR_RL, DIR_FORWARD);
    L298N_SetSpeed(MOTOR_RL, speed);
}

/**
  * @brief  Stop all motors
  */
void L298N_Stop(void)
{
    for (int i = 0; i < MOTOR_COUNT; i++) {
        L298N_SetDirection((Motor_t)i, DIR_STOP);
        L298N_SetSpeed((Motor_t)i, 0);
    }
}
