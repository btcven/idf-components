/**
 * @file main.cpp
 * @author Locha Mesh Developers (locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-11-30
 * 
 * @copyright Copyright (c) 2019 Locha Mesh
 * 
 */
#include "include_bootloader/bootloader_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
void app_main(void);
}

void app_main(void)
{
    while (true) {
        printf("nada por aqui\n");
        //vTaskDelay(1000);
    }
}
