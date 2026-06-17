//
// Created by ded on 4/21/26.
//

#include "main.h"
#include "stm32h7xx_hal_gpio.h"
#include "max7219.h"

// void switch_position_for_set_ampere() {
//     uint8_t btn_s1 = HAL_GPIO_ReadPin(BTN_S1_PE0_GPIO_Port, BTN_S1_PE0_Pin) == GPIO_PIN_SET;
//
//     if (btn_s1 > 0) {
//         if (position_on_mode_edit < 3) {
//             position_on_mode_edit++;
//             if (position_on_mode_edit == 2) {
//                 MAX7219_SendDigitWithDP(positions_ampere_indicate[position_on_mode_edit - 1], buffer_decode_variables_ampere[get_number_for_get_ampere() - 1], CS_GPIO_Port_A, CS_Pin_A15);
//             } else {
//                 MAX7219_SendDigit(positions_ampere_indicate[position_on_mode_edit - 1], buffer_decode_variables_ampere[get_number_for_get_ampere() - 1], CS_GPIO_Port_A, CS_Pin_A15);
//             }
//         } else {
//             MAX7219_SendDigit(positions_ampere_indicate[position_on_mode_edit], buffer_decode_variables_ampere[get_number_for_get_ampere()], CS_GPIO_Port_A, CS_Pin_A15);
//             position_on_mode_edit = 0;
//         }
//     }
// }
// void switch_position_for_set_voltage() {
//     uint8_t btn_s1 = HAL_GPIO_ReadPin(BTN_S1_PE0_GPIO_Port, BTN_S1_PE0_Pin) == GPIO_PIN_SET;
//
//     if (btn_s1 > 0) {
//         if (position_on_mode_edit < 3) {
//             position_on_mode_edit++;
//             if (position_on_mode_edit == 2) {
//                 MAX7219_SendDigitWithDP(positions_voltage_indicate[position_on_mode_edit - 1], buffer_decode_variables_voltage[get_number_for_get_voltage() - 1], CS_GPIO_Port_A, CS_Pin_A15);
//             } else {
//                 MAX7219_SendDigit(positions_voltage_indicate[position_on_mode_edit - 1], buffer_decode_variables_voltage[get_number_for_get_voltage() - 1], CS_GPIO_Port_A, CS_Pin_A15);
//             }
//         } else {
//             MAX7219_SendDigit(positions_voltage_indicate[position_on_mode_edit], buffer_decode_variables_voltage[get_number_for_get_voltage()], CS_GPIO_Port_A, CS_Pin_A15);
//             position_on_mode_edit = 0;
//         }
//     }
// }