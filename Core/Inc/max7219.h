//
// Created by ded on 4/8/26.
//

#ifndef TEST_STM32H743_MAX7219_H
#define TEST_STM32H743_MAX7219_H

#include "main.h"


// Адреса регистров MAX7219
#define MAX7219_REG_NOOP         0x00
// Цифры с первой по восьмую
#define MAX7219_REG_DIGIT0       0x01
#define MAX7219_REG_DIGIT1       0x02
#define MAX7219_REG_DIGIT2       0x03
#define MAX7219_REG_DIGIT3       0x04
#define MAX7219_REG_DIGIT4       0x05
#define MAX7219_REG_DIGIT5       0x06
#define MAX7219_REG_DIGIT6       0x07
#define MAX7219_REG_DIGIT7       0x08
// Функционал типо декодирования, яркости и лимита сканирования
#define MAX7219_REG_DECODE_MODE  0x09
#define MAX7219_REG_INTENSITY    0x0A
#define MAX7219_REG_SCAN_LIMIT   0x0B
// Включение - выключение
#define MAX7219_REG_SHUTDOWN     0x0C
// Принудительное включение всех дисплеев
#define MAX7219_REG_DISPLAY_TEST 0x0F
// Значение размера индикаторов согласно многомерному массиву
// #define INDICATORS_MODE 2
// #define INDICATORS_MODULE 2
// #define INDICATORS_POSITION 4

//Переменные портов
extern GPIO_TypeDef* CS_GPIO_Port_A;
extern GPIO_TypeDef* CS_GPIO_Port_C;
// Переменные пинов
extern uint16_t CS_Pin_A15;
extern uint16_t CS_Pin_C11;
// Массивы
extern uint8_t position_on_mode_edit;

// Функции
void MAX7219_Init(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_Write(uint8_t reg, uint8_t data, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_Clear_decode(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_Clear_not_decode(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_SetIntensity(uint8_t intensity, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
//void MAX7219_SetDigit(uint8_t digit, uint8_t value, uint8_t dp);
void MAX7219_SendDigit(uint8_t position, uint8_t digit, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_SendDigitWithDP(uint8_t position, uint8_t digit, GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_SendEpos(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_SendErr(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX7219_SendDash(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void epos_startup();

uint8_t MAX7219_DetectedPosition(uint8_t param_select, uint8_t indicator_position);
void MAX7219_UpdateDisplay(uint8_t arr_data[2][2][4], uint8_t mode_indicator, uint8_t param_select);

void indicate_set_init(uint8_t arr_data[2][2][4], uint16_t data_ampere, uint16_t data_voltage);

#endif //TEST_STM32H743_MAX7219_H
