/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ax_ps2.h"
#include <stdlib.h>
//#include "stm32f1xx_hal_tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//TIM_HandleTypeDef htim2;// 添加定时器句柄
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 0 */
JOYSTICK_TypeDef ps2;  // 定义PS2手柄结构体变量，用于存储手柄数据

/* USER CODE BEGIN 0 */


/**
  * @brief  初始化电机控制引脚
  * @param  无
  * @retval 无
  */
void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};  // 定义GPIO初始化结构体

    // 使能GPIOA和GPIOB时钟，为后续使用这些端口做准备
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置第一个L298N模块的引脚(IN1, IN2, IN3, IN4)
    GPIO_InitStruct.Pin = L298N1_IN1_PIN | L298N1_IN2_PIN | L298N1_IN3_PIN | L298N1_IN4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速输出
    HAL_GPIO_Init(L298N1_IN1_PORT, &GPIO_InitStruct);  // 初始化第一个L298N的GPIO

    // 配置第二个L298N模块的引脚(IN1, IN2, IN3, IN4)
    GPIO_InitStruct.Pin = L298N2_IN1_PIN | L298N2_IN2_PIN | L298N2_IN3_PIN | L298N2_IN4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速输出
    HAL_GPIO_Init(L298N2_IN1_PORT, &GPIO_InitStruct);  // 初始化第二个L298N的GPIO

    // 初始化所有电机控制引脚为低电平(停止状态)
    HAL_GPIO_WritePin(L298N1_IN1_PORT, L298N1_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N1_IN2_PORT, L298N1_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N1_IN3_PORT, L298N1_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N1_IN4_PORT, L298N1_IN4_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(L298N2_IN1_PORT, L298N2_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N2_IN2_PORT, L298N2_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N2_IN3_PORT, L298N2_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N2_IN4_PORT, L298N2_IN4_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  控制所有电机前进
  * @param  无
  * @retval 无
  */
void Motor_Forward(void)
{
    // 前进 - 所有轮子向前转动
    HAL_GPIO_WritePin(L298N1_IN1_PORT, L298N1_IN1_PIN, GPIO_PIN_SET); // 前左轮前进
    HAL_GPIO_WritePin(L298N1_IN3_PORT, L298N1_IN3_PIN, GPIO_PIN_SET); // 前右轮前进
    HAL_GPIO_WritePin(L298N2_IN1_PORT, L298N2_IN1_PIN, GPIO_PIN_SET); // 后左轮前进
    HAL_GPIO_WritePin(L298N2_IN3_PORT, L298N2_IN3_PIN, GPIO_PIN_SET); // 后右轮前进
}

/**
  * @brief  控制小车左转
  * @param  无
  * @retval 无
  */
//void Motor_TurnLeft(void)
//{
    // 左转 - 左侧轮子向后，右侧轮子向前
//    HAL_GPIO_WritePin(L298N1_IN2_PORT, L298N1_IN2_PIN, GPIO_PIN_SET); // 前左轮后退
//    HAL_GPIO_WritePin(L298N1_IN3_PORT, L298N1_IN3_PIN, GPIO_PIN_SET); // 前右轮前进
//    HAL_GPIO_WritePin(L298N2_IN1_PORT, L298N2_IN1_PIN, GPIO_PIN_SET); // 后左轮前进
//    HAL_GPIO_WritePin(L298N2_IN4_PORT, L298N2_IN4_PIN, GPIO_PIN_SET); // 后右轮后退
//}

/**
  * @brief  控制小车右转
  * @param  无
  * @retval 无
  */
//void Motor_TurnRight(void)
//{
    // 右转 - 左侧轮子向前，右侧轮子向后
//    HAL_GPIO_WritePin(L298N1_IN1_PORT, L298N1_IN1_PIN, GPIO_PIN_SET); // 前左轮前进
//    HAL_GPIO_WritePin(L298N1_IN4_PORT, L298N1_IN4_PIN, GPIO_PIN_SET); // 前右轮后退
//    HAL_GPIO_WritePin(L298N2_IN2_PORT, L298N2_IN2_PIN, GPIO_PIN_SET); // 后左轮后退
//    HAL_GPIO_WritePin(L298N2_IN3_PORT, L298N2_IN3_PIN, GPIO_PIN_SET); // 后右轮前进
//}

/**
  * @brief  停止所有电机
  * @param  无
  * @retval 无
  */
void Motor_Stop(void)
{
    // 将所有电机控制引脚设置为低电平，停止所有电机
    HAL_GPIO_WritePin(L298N1_IN1_PORT, L298N1_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N1_IN2_PORT, L298N1_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N1_IN3_PORT, L298N1_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N1_IN4_PORT, L298N1_IN4_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(L298N2_IN1_PORT, L298N2_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N2_IN2_PORT, L298N2_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N2_IN3_PORT, L298N2_IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L298N2_IN4_PORT, L298N2_IN4_PIN, GPIO_PIN_RESET);
}

void Motor_Backward(void)
{
    // 后退 - 所有轮子向后转动
    HAL_GPIO_WritePin(L298N1_IN2_PORT, L298N1_IN2_PIN, GPIO_PIN_SET); // 前左轮后退
    HAL_GPIO_WritePin(L298N1_IN4_PORT, L298N1_IN4_PIN, GPIO_PIN_SET); // 前右轮后退
    HAL_GPIO_WritePin(L298N2_IN2_PORT, L298N2_IN2_PIN, GPIO_PIN_SET); // 后左轮后退
    HAL_GPIO_WritePin(L298N2_IN4_PORT, L298N2_IN4_PIN, GPIO_PIN_SET); // 后右轮后退
}

/**
  * @brief  控制小车原地左转
  * @param  无
  * @retval 无
  */
void Motor_SpinLeft(void)
{
    // 原地左转 - 左侧轮子向后，右侧轮子向前
    HAL_GPIO_WritePin(L298N1_IN2_PORT, L298N1_IN2_PIN, GPIO_PIN_SET); // 前左轮后退
    HAL_GPIO_WritePin(L298N1_IN3_PORT, L298N1_IN3_PIN, GPIO_PIN_SET); // 前右轮前进
    HAL_GPIO_WritePin(L298N2_IN1_PORT, L298N2_IN1_PIN, GPIO_PIN_SET); // 后左轮前进
    HAL_GPIO_WritePin(L298N2_IN4_PORT, L298N2_IN4_PIN, GPIO_PIN_SET); // 后右轮后退
}

/**
  * @brief  控制小车原地右转
  * @param  无
  * @retval 无
  */
void Motor_SpinRight(void)
{
    // 原地右转 - 左侧轮子向前，右侧轮子向后
    HAL_GPIO_WritePin(L298N1_IN1_PORT, L298N1_IN1_PIN, GPIO_PIN_SET); // 前左轮前进
    HAL_GPIO_WritePin(L298N1_IN4_PORT, L298N1_IN4_PIN, GPIO_PIN_SET); // 前右轮后退
    HAL_GPIO_WritePin(L298N2_IN2_PORT, L298N2_IN2_PIN, GPIO_PIN_SET); // 后左轮后退
    HAL_GPIO_WritePin(L298N2_IN3_PORT, L298N2_IN3_PIN, GPIO_PIN_SET); // 后右轮前进
}


/**
  * @brief  根据摇杆输入控制电机运动
  * @param  x: 摇杆X轴值(-128到127)
  * @param  y: 摇杆Y轴值(-128到127)
  * @retval 无
  */
/**
  * @brief  根据摇杆输入控制电机运动
  * @param  x: 摇杆X轴值(-128到127)
  * @param  y: 摇杆Y轴值(-128到127)
  * @retval 无
  */
void Motor_Control(int8_t x, int8_t y)
{
    // 停止所有电机
    Motor_Stop();

    // 右摇杆只控制前进和后退（Y轴）
    if (abs(y) > 20) {
        if (y > 20) {
            // 向下推摇杆 - 后退
            Motor_Backward();
        } else if (y < -20) {
            // 向上推摇杆 - 前进
            Motor_Forward();
        }
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
    AX_PS2_Init();  // 初始化PS2手柄
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    Motor_Init();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
    // 初始化舵机控制引脚
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        AX_PS2_ScanKey(&ps2);  // 扫描PS2手柄按键和摇杆状态

        // 使用右摇杆控制小车移动
        // ps2.RJoy_LR 控制左右移动 (0x00=左, 0x80=中, 0xFF=右)
        // ps2.RJoy_UD 控制前后移动 (0x00=上, 0x80=中, 0xFF=下)
        int8_t x = (int8_t)(ps2.RJoy_LR - 0x80);  // 转换为有符号数值，中位为0
        int8_t y = (int8_t)(ps2.RJoy_UD - 0x80);  // 转换为有符号数值，中位为0

        // 使用左摇杆控制小车原地旋转
        // ps2.LJoy_LR 控制左右旋转 (0x00=左, 0x80=中, 0xFF=右)
        int8_t lx = (int8_t)(ps2.LJoy_LR - 0x80);  // 转换为有符号数值，中位为0

        // 根据左摇杆值控制原地旋转
        if (abs(lx) > 20) {
            if (lx < -20) {
                // 左摇杆向左 - 原地左转
                Motor_SpinLeft();
            } else if (lx > 20) {
                // 左摇杆向右 - 原地右转
                Motor_SpinRight();
            }
        } else {
            // 左摇杆居中，根据右摇杆控制移动
            Motor_Control(x, y);
        }

        //当按下r1时，位于pa0，pa1,pa2,pa3的舵机sg90转动
        if ((ps2.btn2 & 0x08) == 0) {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,2000);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,2000);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,2000);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,2000);
            HAL_Delay(500);

        }//按r2则反向转动
        else if ((ps2.btn2 & 0x02) == 0) {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,1000);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,1000);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,1000);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,1000);
            HAL_Delay(500);

        }



        HAL_Delay(10);  // 10ms延时，避免过于频繁的扫描
      //测试舵机转动
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 2000);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 2000);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 2000);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 2000);
      // HAL_Delay(1000);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1000);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1000);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1000);
      // HAL_Delay(1000);
      //
      // // 回到中位
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1500);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1500);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1500);
      // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1500);
      // HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
