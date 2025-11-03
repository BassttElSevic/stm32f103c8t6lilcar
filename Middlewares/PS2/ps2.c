/**
  ******************************************************************************
  * @file    ps2.c
  * @brief   PS2 gamepad controller implementation
  * @author  Basstt
  * @note    PS2 controller uses SPI-like protocol
  *          Connections: 
  *          - DAT (MISO): PA6
  *          - CMD (MOSI): PA7
  *          - CLK (SCK):  PA5
  *          - CS (ATT):   PA4
  ******************************************************************************
  */

#include "ps2.h"

/* External SPI handle (defined in main.c) */
extern SPI_HandleTypeDef hspi1;

/* CS pin definitions */
#define PS2_CS_PORT     GPIOA
#define PS2_CS_PIN      GPIO_PIN_4

/* PS2 Commands */
#define PS2_CMD_POLL    0x01
#define PS2_CMD_CONFIG  0x43

/* Private variables */
static uint8_t ps2_tx_buffer[9];
static uint8_t ps2_rx_buffer[9];

/* Private function prototypes */
static void PS2_CS_Low(void);
static void PS2_CS_High(void);
static void PS2_Transfer(uint8_t* tx_data, uint8_t* rx_data, uint8_t length);

/**
  * @brief  Initialize PS2 controller
  */
void PS2_Init(void)
{
    /* CS pin is already configured as GPIO output in main.c */
    PS2_CS_High();
    
    /* Small delay for PS2 controller to stabilize */
    HAL_Delay(100);
}

/**
  * @brief  Read PS2 controller data
  * @param  data: Pointer to PS2_Data_t structure to store the data
  */
void PS2_ReadData(PS2_Data_t* data)
{
    /* Prepare command packet */
    ps2_tx_buffer[0] = 0x01;  /* Start byte */
    ps2_tx_buffer[1] = 0x42;  /* Poll command */
    ps2_tx_buffer[2] = 0x00;  /* Motor control byte 1 */
    ps2_tx_buffer[3] = 0x00;  /* Motor control byte 2 */
    ps2_tx_buffer[4] = 0x00;  /* Padding */
    ps2_tx_buffer[5] = 0x00;  /* Padding */
    ps2_tx_buffer[6] = 0x00;  /* Padding */
    ps2_tx_buffer[7] = 0x00;  /* Padding */
    ps2_tx_buffer[8] = 0x00;  /* Padding */
    
    /* Transfer data */
    PS2_Transfer(ps2_tx_buffer, ps2_rx_buffer, 9);
    
    /* Parse received data */
    if (ps2_rx_buffer[1] == 0x41 || ps2_rx_buffer[1] == 0x73) {
        /* Digital mode (0x41) or Analog mode (0x73) */
        
        /* Button data (inverted - 0 = pressed, 1 = released) */
        data->buttons = ~((ps2_rx_buffer[3] << 8) | ps2_rx_buffer[4]);
        
        /* Analog stick data */
        if (ps2_rx_buffer[1] == 0x73) {
            data->right_x = ps2_rx_buffer[5];
            data->right_y = ps2_rx_buffer[6];
            data->left_x = ps2_rx_buffer[7];
            data->left_y = ps2_rx_buffer[8];
        } else {
            /* Digital mode - set center values */
            data->right_x = 0x80;
            data->right_y = 0x80;
            data->left_x = 0x80;
            data->left_y = 0x80;
        }
    } else {
        /* No controller or invalid response */
        data->buttons = 0;
        data->right_x = 0x80;
        data->right_y = 0x80;
        data->left_x = 0x80;
        data->left_y = 0x80;
    }
}

/**
  * @brief  Check if a button is pressed
  * @param  data: Pointer to PS2_Data_t structure
  * @param  button: Button to check (use PS2_BTN_xxx definitions)
  * @retval 1 if button is pressed, 0 otherwise
  */
uint8_t PS2_IsButtonPressed(PS2_Data_t* data, uint16_t button)
{
    return (data->buttons & button) ? 1 : 0;
}

/**
  * @brief  Pull CS pin low (select PS2 controller)
  */
static void PS2_CS_Low(void)
{
    HAL_GPIO_WritePin(PS2_CS_PORT, PS2_CS_PIN, GPIO_PIN_RESET);
    /* Small delay for signal stabilization (microseconds) */
    for (volatile int i = 0; i < 10; i++);  /* ~1us delay */
}

/**
  * @brief  Pull CS pin high (deselect PS2 controller)
  */
static void PS2_CS_High(void)
{
    HAL_GPIO_WritePin(PS2_CS_PORT, PS2_CS_PIN, GPIO_PIN_SET);
    /* Small delay for signal stabilization (microseconds) */
    for (volatile int i = 0; i < 10; i++);  /* ~1us delay */
}

/**
  * @brief  Transfer data to/from PS2 controller
  * @param  tx_data: Data to transmit
  * @param  rx_data: Buffer for received data
  * @param  length: Number of bytes to transfer
  */
static void PS2_Transfer(uint8_t* tx_data, uint8_t* rx_data, uint8_t length)
{
    PS2_CS_Low();
    
    /* Small delay before transfer */
    HAL_Delay(1);
    
    /* Transfer data via SPI */
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, length, 100);
    
    /* Small delay after transfer */
    HAL_Delay(1);
    
    PS2_CS_High();
}
