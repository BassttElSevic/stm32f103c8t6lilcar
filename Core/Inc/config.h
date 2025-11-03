/**
  ******************************************************************************
  * @file    config.h
  * @brief   Configuration parameters for the car control system
  * @author  Basstt
  ******************************************************************************
  * @attention
  * Modify this file to customize your car's behavior and hardware configuration
  ******************************************************************************
  */

#ifndef __CONFIG_H
#define __CONFIG_H

/* ============================================================================ */
/* MOTOR CONFIGURATION                                                          */
/* ============================================================================ */

/* PWM Configuration */
#define MOTOR_PWM_FREQUENCY     1000    /* PWM frequency in Hz (1kHz) */
#define MOTOR_PWM_PRESCALER     71      /* Timer prescaler (72MHz / 72 = 1MHz) */
#define MOTOR_PWM_PERIOD        999     /* Timer period (1MHz / 1000 = 1kHz) */
#define MOTOR_PWM_MAX_VALUE     1000    /* Maximum PWM value (100% duty cycle) */
#define MOTOR_PWM_MIN_VALUE     0       /* Minimum PWM value (0% duty cycle) */

/* Default Speed Settings */
#define MOTOR_DEFAULT_SPEED     700     /* Default speed (70% of max) */
#define MOTOR_SLOW_SPEED        400     /* Slow speed for precise control */
#define MOTOR_FAST_SPEED        900     /* Fast speed for quick movement */

/* Motor Direction Inversion (set to 1 to invert, 0 for normal) */
#define MOTOR_FL_INVERTED       0       /* Front Left motor */
#define MOTOR_FR_INVERTED       0       /* Front Right motor */
#define MOTOR_RL_INVERTED       0       /* Rear Left motor */
#define MOTOR_RR_INVERTED       0       /* Rear Right motor */

/* ============================================================================ */
/* PS2 CONTROLLER CONFIGURATION                                                 */
/* ============================================================================ */

/* PS2 Communication */
#define PS2_POLLING_INTERVAL    20      /* Polling interval in milliseconds */
#define PS2_SPI_TIMEOUT         100     /* SPI timeout in milliseconds */

/* Analog Stick Dead Zone */
#define PS2_DEADZONE_THRESHOLD  20      /* Dead zone threshold (0-128) */
#define PS2_STICK_CENTER        128     /* Center position of analog stick */

/* Button Configuration */
#define PS2_ENABLE_DPAD         1       /* Enable D-pad control (1=yes, 0=no) */
#define PS2_ENABLE_ANALOG       1       /* Enable analog stick control (1=yes, 0=no) */

/* ============================================================================ */
/* CONTROL MODE CONFIGURATION                                                   */
/* ============================================================================ */

/* Priority: D-pad > Analog (when both enabled) */
#define CONTROL_PRIORITY_DPAD   1       /* D-pad has priority over analog stick */

/* Speed Curve */
#define SPEED_CURVE_LINEAR      1       /* Linear speed curve (1=linear, 0=exponential) */

/* Turn Configuration */
#define TURN_SPEED_FACTOR       100     /* Turn speed as percentage of forward speed */

/* ============================================================================ */
/* DEBUG CONFIGURATION                                                          */
/* ============================================================================ */

/* Debug Features (requires UART implementation) */
#define DEBUG_ENABLE            0       /* Enable debug output */
#define DEBUG_MOTOR_STATUS      0       /* Print motor status */
#define DEBUG_PS2_DATA          0       /* Print PS2 controller data */

/* ============================================================================ */
/* SAFETY CONFIGURATION                                                         */
/* ============================================================================ */

/* Emergency Stop */
#define ENABLE_EMERGENCY_STOP   1       /* Enable emergency stop on signal loss */
#define SIGNAL_LOSS_TIMEOUT     1000    /* Signal loss timeout in milliseconds */

/* Current Limiting */
#define ENABLE_CURRENT_LIMIT    0       /* Enable software current limiting */
#define MAX_MOTOR_CURRENT       2000    /* Maximum motor current in mA */

/* ============================================================================ */
/* HARDWARE PIN CONFIGURATION                                                   */
/* ============================================================================ */

/* L298N #1 (Front Motors) Pin Definitions */
#define L298N1_IN1_PORT         GPIOA
#define L298N1_IN1_PIN          GPIO_PIN_0
#define L298N1_IN2_PORT         GPIOA
#define L298N1_IN2_PIN          GPIO_PIN_1
#define L298N1_IN3_PORT         GPIOA
#define L298N1_IN3_PIN          GPIO_PIN_2
#define L298N1_IN4_PORT         GPIOA
#define L298N1_IN4_PIN          GPIO_PIN_3

/* L298N #2 (Rear Motors) Pin Definitions */
#define L298N2_IN1_PORT         GPIOB
#define L298N2_IN1_PIN          GPIO_PIN_0
#define L298N2_IN2_PORT         GPIOB
#define L298N2_IN2_PIN          GPIO_PIN_1
#define L298N2_IN3_PORT         GPIOB
#define L298N2_IN3_PIN          GPIO_PIN_6
#define L298N2_IN4_PORT         GPIOB
#define L298N2_IN4_PIN          GPIO_PIN_7

/* PS2 Controller Pin Definitions */
#define PS2_CS_PORT             GPIOA
#define PS2_CS_PIN              GPIO_PIN_4
#define PS2_SCK_PORT            GPIOA
#define PS2_SCK_PIN             GPIO_PIN_5
#define PS2_MISO_PORT           GPIOA
#define PS2_MISO_PIN            GPIO_PIN_6
#define PS2_MOSI_PORT           GPIOA
#define PS2_MOSI_PIN            GPIO_PIN_7

/* ============================================================================ */
/* ADVANCED CONFIGURATION                                                       */
/* ============================================================================ */

/* Battery Monitor (requires ADC implementation) */
#define ENABLE_BATTERY_MONITOR  0       /* Enable battery voltage monitoring */
#define BATTERY_LOW_VOLTAGE     700     /* Low battery voltage in 0.01V (7.00V) */
#define BATTERY_CRITICAL_VOLTAGE 650    /* Critical battery voltage in 0.01V (6.50V) */

/* Additional Features */
#define ENABLE_LED_INDICATOR    0       /* Enable LED status indicator */
#define ENABLE_BUZZER           0       /* Enable buzzer for alerts */

#endif /* __CONFIG_H */
