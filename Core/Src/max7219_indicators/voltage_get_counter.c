#include <stdint.h>

#include "max7219.h"
//
// Created by ded on 4/21/26.
//
float test_voltage = 12.34;
uint8_t positions_voltage_indicate[4] = {7, 5, 1, 0};
uint8_t buffer_decode_variables_voltage[4] = {0};

void indicate_voltage_get_init(float voltage_value) {
    MAX7219_Write(MAX7219_REG_DECODE_MODE, 0xFF, CS_GPIO_Port_A, CS_Pin_A15);

    uint16_t work_value = voltage_value * 100 + 1;

    buffer_decode_variables_voltage[3] = work_value % 10;
    buffer_decode_variables_voltage[2] = work_value / 100 % 10;
    buffer_decode_variables_voltage[1] = work_value / 1000 % 100;
    buffer_decode_variables_voltage[0] = work_value / 1000;

    MAX7219_SendDigit(7, buffer_decode_variables_voltage[0], CS_GPIO_Port_A, CS_Pin_A15);
    MAX7219_SendDigitWithDP(5, buffer_decode_variables_voltage[1], CS_GPIO_Port_A, CS_Pin_A15);
    MAX7219_SendDigit(1, buffer_decode_variables_voltage[2], CS_GPIO_Port_A, CS_Pin_A15);
    MAX7219_SendDigit(0, buffer_decode_variables_voltage[3], CS_GPIO_Port_A, CS_Pin_A15);
}

uint8_t set_number_for_get_voltage() {

    for (uint8_t i = 0; i < 4; i++) {
        if (i == position_on_mode_edit) {
            return positions_voltage_indicate[position_on_mode_edit];
        }
    }

}

uint8_t get_number_for_get_voltage() {

    uint8_t work_number = set_number_for_get_voltage();

    switch (work_number) {
        case 7:
            return 0;
            break;
        case 5:
            return 1;
            break;
        case 1:
            return 2;
            break;
        case 0:
            return 3;
            break;
    }
}