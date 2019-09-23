
/**
 * @file i2c.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define SDA_PIN 18
#define SCL_PIN 19
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define I2C_MASTER_SCL_IO 19               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 18               /*!< gpio number for I2C master data  */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1  
#define DATA_LENGTH 512 
#define GPIO_INPUT_IO_0  4
#define GLCD_SLAVE_ADDR 0x20
#define PIN_5_E 25
#define PIN_6_R 27
#define GPIO_OUTPUT_PIN_SEL    ((1ULL<<PIN_5_E)  || (1ULL<<PIN_6_R))

