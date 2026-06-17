//
// Created by ded on 4/13/26.
//

#include "main.h"
#include "encoder_functional.h"
#include "max7219.h"

uint8_t encoder_1 = 0;
uint8_t encoder_2 = 0;
uint8_t status_mode = 0;

void encoder_1_init(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value) {
    encoder_1 = HAL_GPIO_ReadPin(ENC_B1_GPIO_Port, ENC_B1_Pin);

        if (encoder_1 > 0) {
            encoder_right_spin(arr_data, mod_value, param_value, position_on_mode_edit);
        } else {
            encoder_left_spin(arr_data, mod_value, param_value, position_on_mode_edit);
        }
}

void encoder_2_init(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value) {
    encoder_2 = HAL_GPIO_ReadPin(ENC_B2_GPIO_Port, ENC_B2_Pin);

    if (encoder_2 > 0) {
        encoder_right_spin(arr_data, mod_value, param_value, position_on_mode_edit);
    } else {
        encoder_left_spin(arr_data, mod_value, param_value, position_on_mode_edit);
    }
}

void encoder_left_spin(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value, uint8_t pos) {

    if (pos == 0) {
        // Для arr[0] просто уменьшаем, если больше 0
        if (arr_data[mod_value][param_value][0] > 0) {
            arr_data[mod_value][param_value][0]--;
        }
        return;
    }

    // Для индексов 1, 2, 3
    if (pos == 1) {
        // Уменьшаем arr[1]
        if (arr_data[mod_value][param_value][1] > 0) {
            arr_data[mod_value][param_value][1]--;
        } else {
            // arr[1] == 0, нужно занять у arr[0]
            if (arr_data[mod_value][param_value][0] > 0) {
                arr_data[mod_value][param_value][1] = 9;
                encoder_left_spin(arr_data, mod_value, param_value, 0);
            }
            // Если arr[0] == 0, то arr[1] остается 0 (нельзя уменьшить)
        }
        return;
    }

    if (pos == 2) {
        if (arr_data[mod_value][param_value][2] > 0) {
            arr_data[mod_value][param_value][2]--;
        } else {
            // Значение == 0, нужно занять
            arr_data[mod_value][param_value][2] = 9;

            // Проверяем, можно ли занять у arr[1]
            if (arr_data[mod_value][param_value][1] > 0) {
                encoder_left_spin(arr_data, mod_value, param_value, 1);  // Уменьшаем arr[1]
            } else {
                // Если arr[1] == 0, проверяем arr[0]
                if (arr_data[mod_value][param_value][0] > 0) {
                    // Уменьшаем arr[0] (каскад через arr[1])
                    encoder_left_spin(arr_data, mod_value, param_value, 1);  // Это уменьшит arr[1] с 0 до 9 и вызовет уменьшение arr[0]
                }
                // Если arr[0] тоже 0, то оставляем как есть (нельзя уменьшить)
            }
        }
        return;
    }

    if (pos == 3) {
        if (arr_data[mod_value][param_value][3] > 0) {
            arr_data[mod_value][param_value][3]--;
        } else {
            // Значение == 0, нужно занять
            arr_data[mod_value][param_value][3] = 9;

            // Проверяем, можно ли занять у arr[2]
            if (arr_data[mod_value][param_value][2] > 0) {
                encoder_left_spin(arr_data, mod_value, param_value, 2);  // Уменьшаем arr[2]
            } else {
                // Если arr[2] == 0, проверяем arr[1]
                if (arr_data[mod_value][param_value][1] > 0) {
                    encoder_left_spin(arr_data, mod_value, param_value, 2);  // Рекурсия через arr[2] дойдёт до arr[1]
                } else {
                    // Если arr[1] == 0, проверяем arr[0]
                    if (arr_data[mod_value][param_value][0] > 0) {
                        encoder_left_spin(arr_data, mod_value, param_value, 2);  // Рекурсия сама всё обработает
                    }
                    // Если все равны 0, то ничего не делаем
                }
            }
        }
        return;
    }


    // if (pos == 2) {
    //     arr_data[mod_value][param_value][2]--;
    //
    //     if (arr_data[mod_value][param_value][2] < 0) {
    //         arr_data[mod_value][param_value][2] = 9;
    //         encoder_left_spin(arr_data, mod_value, param_value, 1);  // Занимаем у arr[1]
    //     }
    //     return;
    // }
    //
    // if (pos == 3) {
    //     arr_data[mod_value][param_value][3]--;
    //
    //     if (arr_data[mod_value][param_value][3] < 0) {
    //         arr_data[mod_value][param_value][3] = 9;
    //         encoder_left_spin(arr_data, mod_value, param_value, 2);  // Занимаем у arr[2]
    //     }
    //     return;
    // }

}
void encoder_right_spin(uint8_t arr_data[2][2][4], uint8_t mod_value, uint8_t param_value, uint8_t pos) {

    if (pos == 0) {
        // Правила для arr[0]
        int max_val;
        if (arr_data[mod_value][param_value][1] > 0) {
            max_val = 2;  // Если arr[1] > 0, то arr[0] не больше 2
        } else {
            max_val = 3;  // Если arr[1] == 0, то arr[0] может быть до 3
        }

        if (arr_data[mod_value][param_value][0] < max_val) {
            arr_data[mod_value][param_value][0]++;
        }
        return;
    }

    // Для индексов 1, 2, 3
    if (pos == 1) {
        // Особое правило: если arr[0] == 3, то arr[1] не может быть больше 0
        if (arr_data[mod_value][param_value][0] == 3) {
            return;
        }

        arr_data[mod_value][param_value][1]++;

        // Если arr[1] превысил 9, переносим в arr[0]
        if (arr_data[mod_value][param_value][1] > 9) {
            arr_data[mod_value][param_value][1] = 0;
            encoder_right_spin(arr_data, mod_value, param_value, 0);  // Увеличиваем arr[0]
        }
        return;
    }

    if (pos == 2) {
        arr_data[mod_value][param_value][2]++;

        if (arr_data[mod_value][param_value][2] > 9) {
            arr_data[mod_value][param_value][2] = 0;
            encoder_right_spin(arr_data, mod_value, param_value, 1);  // Перенос в arr[1]
        }
        return;
    }

    if (pos == 3) {
        arr_data[mod_value][param_value][3]++;

        if (arr_data[mod_value][param_value][3] > 9) {
            arr_data[mod_value][param_value][3] = 0;
            encoder_right_spin(arr_data, mod_value, param_value, 2);  // Перенос в arr[2]
        }
        return;
    }

}
