//
// Created by ded on 4/10/26.
//
#include "main.h"

/*
 * status values:
 * 1 - green
 * 2 - red
 * 3 - green + red
 * 4 - off
 */

void led1_set_state(uint8_t status) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // Default value LED1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // Default value LED1
    switch (status) {
        case 1:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
            break;
        case 2:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
            break;
        case 3:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // Default value LED1
            break;
    }

}

void led2_set_state(uint8_t status) {

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // Default value LED1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // Default value LED1

    switch (status) {
        case 1:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
            break;
        case 2:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
            break;
        case 3:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // Default value LED1
            break;
    }
}

void led3_set_state(uint8_t status) {

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); // Default value LED1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // Default value LED1

    switch (status) {
        case 1:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
            break;
        case 2:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
            break;
        case 3:
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // Default value LED1
            break;
    }
}