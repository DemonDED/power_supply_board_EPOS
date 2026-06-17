//
// Created by ded on 4/10/26.
//
#include "max7219.h"

uint8_t position_on_mode_edit = 0;

void indicate_set_init(uint8_t arr_data[2][2][4], uint16_t data_ampere, uint16_t data_voltage) {
    MAX7219_Write(MAX7219_REG_DECODE_MODE, 0xFF, CS_GPIO_Port_A, CS_Pin_A15);

    // SET display
    arr_data[0][0][0] = data_ampere / 1000;
    arr_data[0][0][1] = data_ampere % 1000 / 100;
    arr_data[0][0][2] = data_ampere % 100 / 10;
    arr_data[0][0][3] = data_ampere % 10 / 1;

    arr_data[0][1][0] = data_voltage / 1000;
    arr_data[0][1][1] = data_voltage % 1000 / 100;
    arr_data[0][1][2] = data_voltage % 100 / 10;
    arr_data[0][1][3] = data_voltage % 10 / 1;

    //GET display
    // arr_data[1][0][0]
}
