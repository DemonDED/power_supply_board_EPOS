//
// Created by ded on 4/13/26.
//

#ifndef TEST_STM32H743_ENCODER_FUNCTIONAL_H
#define TEST_STM32H743_ENCODER_FUNCTIONAL_H

// Values
extern uint8_t status_mode;
// Functions
void encoder_1_init(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value);
void encoder_2_init(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value);
void encoder_left_spin(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value, uint8_t pos);
void encoder_right_spin(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value, uint8_t pos);

#endif //TEST_STM32H743_ENCODER_FUNCTIONAL_H
