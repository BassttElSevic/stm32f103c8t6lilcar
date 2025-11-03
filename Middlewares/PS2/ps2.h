/**
  ******************************************************************************
  * @file    ps2.h
  * @brief   PS2 gamepad controller interface
  * @author  Basstt
  ******************************************************************************
  */

#ifndef __PS2_H
#define __PS2_H

#include "stm32f1xx_hal.h"

/* PS2 Button definitions */
#define PS2_BTN_SELECT      0x0001
#define PS2_BTN_L3          0x0002
#define PS2_BTN_R3          0x0004
#define PS2_BTN_START       0x0008
#define PS2_BTN_UP          0x0010
#define PS2_BTN_RIGHT       0x0020
#define PS2_BTN_DOWN        0x0040
#define PS2_BTN_LEFT        0x0080
#define PS2_BTN_L2          0x0100
#define PS2_BTN_R2          0x0200
#define PS2_BTN_L1          0x0400
#define PS2_BTN_R1          0x0800
#define PS2_BTN_TRIANGLE    0x1000
#define PS2_BTN_CIRCLE      0x2000
#define PS2_BTN_CROSS       0x4000
#define PS2_BTN_SQUARE      0x8000

/* PS2 Controller data structure */
typedef struct {
    uint16_t buttons;       /* Button state (1 = pressed, 0 = released) */
    uint8_t right_x;        /* Right analog stick X (0-255) */
    uint8_t right_y;        /* Right analog stick Y (0-255) */
    uint8_t left_x;         /* Left analog stick X (0-255) */
    uint8_t left_y;         /* Left analog stick Y (0-255) */
} PS2_Data_t;

/* Function prototypes */
void PS2_Init(void);
void PS2_ReadData(PS2_Data_t* data);
uint8_t PS2_IsButtonPressed(PS2_Data_t* data, uint16_t button);

#endif /* __PS2_H */
