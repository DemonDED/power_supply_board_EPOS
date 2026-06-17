//
// Created by ded on 4/9/26.
//
//
// Created by ded on 4/8/26.
//
#include "max7219.h"
#include "stm32h743xx.h"
#include "spi.h"

GPIO_TypeDef* CS_GPIO_Port_A = GPIOA;
GPIO_TypeDef* CS_GPIO_Port_C = GPIOC;

uint16_t CS_Pin_A15 = GPIO_PIN_15;
uint16_t CS_Pin_C11 = GPIO_PIN_11;



void MAX7219_Write(uint8_t reg, uint8_t data, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    uint16_t command = (reg << 8) | data;
    HAL_StatusTypeDef status;

    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

    status = HAL_SPI_Transmit(&hspi3, (uint8_t *)&command, 1, 100);

    if (status != HAL_OK) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
        while(1);  // Останавливаемся
    }

    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

// Инициализация MAX7219
void MAX7219_Init(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    // Отключение тестового режима
    MAX7219_Write(MAX7219_REG_DISPLAY_TEST, 0x00, CS_GPIO_Port, CS_Pin);

    // Выход из режима shutdown в нормальный режим
    MAX7219_Write(MAX7219_REG_SHUTDOWN, 0x01, CS_GPIO_Port, CS_Pin);

    // Декодирование: 0xFF - для всех цифр, 0x00 - без декодера (для матрицы)
    // Для 7-сегментных индикаторов используем 0xFF
    MAX7219_Write(MAX7219_REG_DECODE_MODE, 0xFF, CS_GPIO_Port, CS_Pin);  // С декодером

    // Яркость: 0x00 (мин) ... 0x0F (макс)
    MAX7219_Write(MAX7219_REG_INTENSITY, 0x0F, CS_GPIO_Port, CS_Pin);

    // Количество используемых цифр: 0x00 (1 цифра) ... 0x07 (8 цифр)
    MAX7219_Write(MAX7219_REG_SCAN_LIMIT, 0x07, CS_GPIO_Port, CS_Pin);

    // Очистка дисплея
    MAX7219_Clear_decode(CS_GPIO_Port, CS_Pin);
}

// Очистка всех цифр
void MAX7219_Clear_decode(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        MAX7219_Write(MAX7219_REG_DIGIT0 + i, 0x0F, CS_GPIO_Port, CS_Pin);
    }
}
void MAX7219_Clear_not_decode(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    for (uint8_t i = 0; i < 8; i++) {
        MAX7219_Write(MAX7219_REG_DIGIT0 + i, 0x00, CS_GPIO_Port, CS_Pin);
    }
}

// Установка яркости (0-15)
void MAX7219_SetIntensity(uint8_t intensity, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    if (intensity > 15) intensity = 15;
    MAX7219_Write(MAX7219_REG_INTENSITY, intensity, CS_GPIO_Port, CS_Pin);
}

// Установка значения для конкретной цифры (режим NO DECODE)
// digit: 0-7 (0 - крайняя левая)
// data: 8 бит для сегментов (бит 7 = DP, бит 0-6 = A-G)
// dp: включить десятичную точку
// void MAX7219_SetDigit(uint8_t digit, uint8_t data, uint8_t dp)
// {
//     if (digit > 7) return;
//     if (dp) data |= 0x80;  // Установить бит DP (старший бит)
//     MAX7219_Write(MAX7219_REG_DIGIT0 + digit, data);
// }
// Простая отправка цифры (0-9, или 0x0F для пустого)
void MAX7219_SendDigit(uint8_t position, uint8_t digit, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    if (position > 7) return;

    // В режиме декодирования MAX7219 принимает:
    // 0x00-0x09 = цифры 0-9
    // 0x0A = тире (-)
    // 0x0A = буква E
    // 0x0B = буква H
    // 0x0C = буква L
    // 0x0D = буква P
    // 0x0F = пусто (гасит индикатор)

    MAX7219_Write(MAX7219_REG_DIGIT0 + position, digit, CS_GPIO_Port, CS_Pin);
}

// Расширенная функция с десятичной точкой
void MAX7219_SendDigitWithDP(uint8_t position, uint8_t digit, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    if (position > 7) return;

    // В режиме декодирования десятичная точка включается установкой старшего бита
    // Например: 0x81 = цифра 1 с десятичной точкой
    if (digit <= 9) {
        MAX7219_Write(MAX7219_REG_DIGIT0 + position, digit | 0x80, CS_GPIO_Port, CS_Pin);
    }
}

void MAX7219_SendEpos(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin) {
    MAX7219_Clear_decode(CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DECODE_MODE, 0x00, CS_GPIO_Port, CS_Pin); // Decode Off
    MAX7219_Clear_not_decode(CS_GPIO_Port, CS_Pin);
    HAL_Delay(1000);

    MAX7219_Write(MAX7219_REG_DIGIT0, 0b01011011, CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DIGIT1, 0b01111110, CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DIGIT5, 0b01100111, CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DIGIT7, 0b01001111, CS_GPIO_Port, CS_Pin);
}
void MAX7219_SendErr(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    MAX7219_Clear_decode(CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DECODE_MODE, 0x00, CS_GPIO_Port, CS_Pin); // Decode Off
    MAX7219_Clear_not_decode(CS_GPIO_Port, CS_Pin);
    HAL_Delay(1000);

    MAX7219_Write(MAX7219_REG_DIGIT0, 0b01000110, CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DIGIT1, 0b01000110, CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DIGIT5, 0b01001111, CS_GPIO_Port, CS_Pin);
}
void MAX7219_SendDash(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
    MAX7219_Clear_not_decode(CS_GPIO_Port, CS_Pin);
    MAX7219_Write(MAX7219_REG_DECODE_MODE, 0xFF, CS_GPIO_Port, CS_Pin);
    MAX7219_Clear_decode(CS_GPIO_Port, CS_Pin);

    MAX7219_SendDigit(7, 0x0A, CS_GPIO_Port, CS_Pin);
    MAX7219_SendDigit(5, 0x0A, CS_GPIO_Port, CS_Pin);
    MAX7219_SendDigit(1, 0x0A, CS_GPIO_Port, CS_Pin);
    MAX7219_SendDigit(0, 0x0A, CS_GPIO_Port, CS_Pin);
}

uint8_t MAX7219_DetectedPosition(uint8_t param_select, uint8_t indicator_position) {
    switch (param_select) {
        case 0:
            switch (indicator_position) {
            case 0:
                    return 6;
            case 1:
                    return 2;
            case 2:
                    return 3;
            case 3:
                    return 4;
            }
        case 1:
            switch (indicator_position) {
            case 0:
                    return 7;
            case 1:
                    return 5;
            case 2:
                    return 1;
            case 3:
                    return 0;
            }
    }
}

void MAX7219_UpdateDisplay(uint8_t arr_data[2][2][4], uint8_t mode_indicator, uint8_t param_select) {
    switch (mode_indicator) {
        case 0:
            // switch (param_select) {
                // case 0:
                    for (uint8_t i = 0; i < 4; i++) {
                        if (i == 1) {
                            MAX7219_SendDigitWithDP(MAX7219_DetectedPosition(param_select, i), arr_data[mode_indicator][param_select][i], CS_GPIO_Port_A, CS_Pin_A15);
                        } else {
                            MAX7219_SendDigit(MAX7219_DetectedPosition(param_select, i), arr_data[mode_indicator][param_select][i], CS_GPIO_Port_A, CS_Pin_A15);
                        }
                    }
                    // break;
                // case 1:
            // }

            break;
        case 1:
            // switch (param_select) {
                // case 0:
            for (uint8_t i = 0; i < 4; i++) {
                if (i == 1) {
                    MAX7219_SendDigitWithDP(MAX7219_DetectedPosition(param_select, i), arr_data[mode_indicator][param_select][i], CS_GPIO_Port_C, CS_Pin_C11);
                } else {
                    MAX7219_SendDigit(MAX7219_DetectedPosition(param_select, i), arr_data[mode_indicator][param_select][i], CS_GPIO_Port_C, CS_Pin_C11);
                }
            }
            // }
    }
}
