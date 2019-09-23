/**
 * @file main.cpp
 * @author omer (omerceylan38@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "i2c_scanner.h"
#include <string>


static esp_err_t i2c_master_init()
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)26;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)25;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config((i2c_port_t)i2c_master_port, &conf);
    return i2c_driver_install((i2c_port_t)i2c_master_port, conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t i2c_read(i2c_port_t i2c_num, uint8_t address, uint8_t *data1)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, address << 1 | READ_BIT, (i2c_ack_type_t)ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data1, (i2c_ack_type_t)NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        return ret;
    }
    return ret;
}

esp_err_t i2c_write(i2c_port_t i2c_num, uint8_t address, uint8_t data)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, address << 1 | WRITE_BIT, (i2c_ack_type_t)ACK_CHECK_EN);
    i2c_master_write_byte(cmd, address, (i2c_ack_type_t)ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data, (i2c_ack_type_t)ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK)
    {
        return ret;
    }
    return ret;
}

void i2c_Scanner_Task(void *arg)
{

    esp_err_t espRc;
    for (size_t i = 0; i < 0x7F; i++)
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 /*expect ack */);
        i2c_master_stop(cmd);

        espRc = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 100 / portTICK_PERIOD_MS);
        switch (espRc)
        {
        case ESP_OK:
            printf("Success %d\n", i);
            break;
        case ESP_ERR_INVALID_ARG:
            printf("Parameter error\n");
            break;
        case ESP_FAIL:
            printf("Sending command error, slave doesn't ACK the transfer.\n");
            break;
        case ESP_ERR_INVALID_STATE:
            printf("I2C driver not installed or not in master mode.\n");
            break;
        case ESP_ERR_TIMEOUT:
            printf("Operation timeout because the bus is busy.\n");
            break;
        default:
            printf("default\n");
            break;
        }
        printf("i = %d -", i);
        i2c_cmd_link_delete(cmd);
    }
    printf("\n");
    vTaskDelete(NULL);
}

extern "C" void app_main()
{
    i2c_master_init();
    xTaskCreate(i2c_Scanner_Task, "i2c_Scanner_Task", 2048, NULL, 10, NULL);
}
