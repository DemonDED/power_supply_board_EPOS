//
// Created by Alekseev on 27.10.2021.
//

#ifndef MB_H
#define MB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MB_cfg.h"
#include "stdint.h"
#include "main.h"
#include "stdbool.h"

#define MB_FUNCTION_F3 0x03
#define MB_FUNCTION_F6 0x06
#define MB_FUNCTION_F16 0x10

#define MB_BUFFER_RW_SIZE_MAX (MB_BUFFER_SIZE_MAX * 2)

typedef uint16_t MB_byteCounter;

typedef enum {
    MB_OK = 0,
    MB_ERROR
} MB_status;

typedef enum {
    MB_P_COMPLETE = 0,
    MB_P_WAIT,
    MB_P_RESET,
} MB_packetStatus;

typedef struct {
    uint16_t buffer;
    uint16_t state;
} MB_bufferAndState_h;

typedef struct {
#if MB_BUFFER_COIL_SIZE_MAX
    MB_bufferAndState_h coil[MB_BUFFER_COIL_SIZE_MAX];
#endif
#if MB_BUFFER_DISCRETE_SIZE_MAX
    MB_bufferAndState_h discrete[MB_BUFFER_DISCRETE_SIZE_MAX];
#endif
#if MB_BUFFER_HOLDING_SIZE_MAX
    MB_bufferAndState_h holding[MB_BUFFER_HOLDING_SIZE_MAX];
#endif
#if MB_BUFFER_INPUT_SIZE_MAX
    MB_bufferAndState_h input[MB_BUFFER_INPUT_SIZE_MAX];
#endif
} MB_buffer_h;

typedef struct {
    UART_HandleTypeDef *huart;
    GPIO_TypeDef *GPIOx;
    uint16_t pin;
} MB_hard_h;

typedef struct {
    MB_hard_h hard;
    uint8_t rxBuffer[MB_BUFFER_RW_SIZE_MAX];
    uint8_t txBuffer[MB_BUFFER_RW_SIZE_MAX];
    MB_buffer_h buffer;
    MB_packetStatus packetStatus;
    MB_byteCounter byteCount;
    uint8_t deviceAddress;
    volatile uint16_t timeOut;
    volatile uint16_t timeGap;
} MB_general_h;

void MB_init(MB_general_h *p_hgs);
void MB_itTime(MB_general_h *p_hgs);
void MB_it(MB_general_h *p_hgs);
void MB_checkFrame(MB_general_h *p_hgs);
bool MB_checkRegister(MB_bufferAndState_h *p_hbs, uint16_t address);
uint16_t MB_getRegister(MB_bufferAndState_h *p_hbs, uint16_t address);
void MB_setRegister(MB_bufferAndState_h *p_hbs, uint16_t address, uint16_t data);

MB_status MB_f3Send(MB_general_h *p_hgs,
                    uint8_t slaveAddress,
                    uint16_t beginRegisterAddress,
                    uint16_t registerCount);
MB_status MB_f6Send(MB_general_h *p_hgs,
                    uint8_t slaveAddress,
                    uint16_t registerAddress,
                    uint16_t data);
MB_status MB_f16Send(MB_general_h *p_hgs,
                    uint8_t slaveAddress,
                    uint16_t beginRegisterAddress,
                    uint16_t registerCount);

#ifdef __cplusplus
}
#endif

#endif //MB_H
