/***
* @file    M95.h
* @brief   This file contains all the functions prototypes for the M95 EEPROM
*         firmware library.
 */

#ifndef INC_M_95_H_
#define INC_M_95_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"


#define EEPROM_WREN  0x06  /*!< Write Enable */
#define EEPROM_WRDI  0x04  /*!< Write Disable */
#define EEPROM_RDSR  0x05  /*!< Read Status Register */
#define EEPROM_WRSR  0x01  /*!< Write Status Register */
#define EEPROM_READ  0x03  /*!< Read from Memory Array */
#define EEPROM_WRITE 0x02  /*!< Write to Memory Array */

#define EEPROM_WIP_FLAG        0x01  /*!< Write In Progress (WIP) flag */

#define EEPROM_PAGESIZE        64    /*!< Pagesize according to documentation */
//#define EEPROM_BUFFER_SIZE     32    /*!< EEPROM Buffer size. Setup to your needs */

#define EEPROM_CS_HIGH()    HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_SET)
#define EEPROM_CS_LOW()     HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_RESET)

void EEPROM_ErrorHandler(void);

void sEE_WriteEnable(void);

void sEE_WriteDisable(void);

void sEE_WriteStatusRegister(uint8_t regval);

uint8_t EEPROM_WaitStandbyState(void);

void EEPROM_SendInstruction(uint8_t *instruction, uint8_t size);

typedef enum {
    EEPROM_STATUS_PENDING,
    EEPROM_STATUS_COMPLETE,
    EEPROM_STATUS_ERROR
} EepromOperations;

EepromOperations EEPROM_WritePage(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);

EepromOperations EEPROM_WriteBuffer(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);

EepromOperations EEPROM_ReadBuffer(uint8_t *pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);

void EEPROM_init();

#ifdef __cplusplus
}
#endif

#endif /* INC_M_95_H_ */
