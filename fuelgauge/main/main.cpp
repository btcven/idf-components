/**
 * 
 * @file main.cpp
 * @author Locha Mesh Developers (locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-11-30
 * 
 * @copyright Copyright (c) 2019 Locha Mesh
 */
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"

#include "constants.h"
#include <cstdlib>
#include <cstring>
#include <driver/gpio.h>
#include <driver/i2c.h>

i2c_mode_t i2c_mode = I2C_MODE_MASTER;
i2c_port_t i2c_port = I2C_NUM_0;
gpio_num_t sda_pin = GPIO_NUM_23;
gpio_num_t scl_pin = GPIO_NUM_22;
gpio_pullup_t sda_pullup = GPIO_PULLUP_DISABLE;
gpio_pullup_t scl_pullup = GPIO_PULLUP_DISABLE;

uint32_t i2c_frequency = 100000;

uint32_t ticksToWait = 10;

esp_err_t i2c_init(void)
{
    i2c_port_t i2c_master_port = i2c_port;
    // **
    i2c_config_t conf;
    conf.mode = i2c_mode;
    conf.sda_io_num = sda_pin;
    conf.sda_pullup_en = sda_pullup;
    conf.scl_io_num = scl_pin;
    conf.scl_pullup_en = scl_pullup;
    conf.master.clk_speed = i2c_frequency;

    esp_err_t err;
    // **
    err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "\t i2c config [ERROR] %s", esp_err_to_name(err));
        return err;
    }
    // **
    err = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "\t i2c driver install [ERROR] %s", esp_err_to_name(err));
    }
    return err;
}
esp_err_t i2c_delete(void)
{
    i2c_port_t i2c_master_port = i2c_port;
    esp_err_t res = i2c_driver_delete(i2c_master_port);
    return res;
}

esp_err_t i2c_check(uint8_t addr, int32_t timeout)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK_EN);
    i2c_master_stop(cmd);
    esp_err_t res = i2c_master_cmd_begin(i2c_port, cmd, (timeout < 0 ? ticksToWait : pdMS_TO_TICKS(timeout)));
    i2c_cmd_link_delete(cmd);
    return res;
}

void i2c_detect(void)
{
    int32_t scanTimeout = 20;
    uint8_t j = 0;
    for (size_t i = 0x40; i < 0x70; i++) {
        if (i2c_check(i, scanTimeout) == ESP_OK) {
            ESP_LOGI(__func__, "\t ** Device found at 0x%X **", i);
            j++;
        }
    }
    if (j == 0) {
        ESP_LOGE(__func__, "\t ** No devices found **");
    }
}

uint8_t readByte(uint8_t command, int32_t timeout)
{
    // ToDo
    return 0;
}

uint16_t readWord(uint8_t command, int32_t timeout)
{
    uint8_t data[2];

    // init i2c interface
    i2c_init();
    // set timeout
    i2c_set_timeout(i2c_port, 14000);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // i2c start
    i2c_master_start(cmd);
    // Select the slave by address
    i2c_master_write_byte(cmd, (0x55 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, command, true);
    //
    // Start read
    i2c_master_start(cmd);
    // Select the slave by addr
    i2c_master_write_byte(cmd, (0x55 << 1) | I2C_MASTER_READ, true);
    // Read data from slave
    i2c_master_read(cmd, data, 2, I2C_MASTER_LAST_NACK);
    // Stop transaction
    i2c_master_stop(cmd);
    // Begin transaction
    esp_err_t res = i2c_master_cmd_begin(i2c_port, cmd, (timeout < 0 ? ticksToWait : pdMS_TO_TICKS(timeout)));
    if (res != ESP_OK) {
        ESP_LOGE(__func__, "\t i2c_master_cmd_begin -- [ERROR] %s", esp_err_to_name(res));
    }
    // i2c driver delete
    i2c_delete();

    return (data[1] << 8) | data[0];
}

void voltage(void)
{
    int16_t self = readWord(0x04, 2000);
    printf("Voltage: ");
    printf("%d mV\n", self);
}

void avg_current(void)
{
    int16_t self = readWord(0x10, 2000);
    printf("Avg. Current: ");
    printf("%d mA\n", self);
}

void max_current(void)
{
    int16_t self = readWord(0x14, 2000);
    printf("Max. Current: ");
    printf("%d mA\n", self);
}

void avg_power(void)
{
    int16_t self = readWord(0x18, 2000);
    printf("Avg. Power: ");
    printf("%d mW\n", self);
}

extern "C" void
app_main(void)
{
    voltage();
    avg_current();
    max_current();
    avg_power();
}
