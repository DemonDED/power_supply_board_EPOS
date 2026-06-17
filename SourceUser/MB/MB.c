/**
 * @file MB.c
 * @author Alekseev S. A.
 * @date 27.10.2021
 * @version 1.0.0
 * @brief This file provides controls for the following functions
 * @verbatim
 * Поддержка функций: F3 F6
 *      Для реализации master и slave
 * @endverbatim
 */

#include "MB.h"
#include "MB_rw.h"

#define U8D_TO_U16(hi, lo) (uint16_t)(hi << 8 | lo)
#define U16_TO_U8H(u16) (u16 >> 8)
#define U16_TO_U8L(u16) (u16 & 0xFF)

static uint16_t MB_getCrc16(uint8_t *buf, uint16_t length);
static bool MB_checkCrc16Rx(MB_general_h *p_hgs);
static bool MB_checkCrc16Len(MB_general_h *p_hgs, uint16_t length);

static void MB_frameRead(MB_general_h *p_hgs);
static void MB_frameReset(MB_general_h *p_hgs);
static void MB_rxBufferClear(MB_general_h *p_hgs);

static void MB_f3(MB_general_h *p_hgs);
static void MB_f6(MB_general_h *p_hgs);
static void MB_f16(MB_general_h *p_hgs);

void MB_init(MB_general_h *p_hgs) {
    MB_rxBufferClear(p_hgs);
    MB_frameReset(p_hgs);
    MB_frameRead(p_hgs);
}

void MB_itTime(MB_general_h *p_hgs) {
    if (p_hgs->timeOut) --p_hgs->timeOut;
    if (p_hgs->timeGap) {
        --p_hgs->timeGap;
        if (p_hgs->packetStatus != MB_P_WAIT) p_hgs->packetStatus = MB_P_WAIT;
    } else {
        if (p_hgs->packetStatus == MB_P_WAIT) {
            p_hgs->packetStatus = MB_P_COMPLETE;
        }
    }
}

void MB_it(MB_general_h *p_hgs) {
    MB_frameRead(p_hgs);
    p_hgs->timeGap = MB_PACKET_TIME_GAP;
}

void MB_checkFrame(MB_general_h *p_hgs) {
    if (p_hgs->packetStatus == MB_P_COMPLETE) {
        if ((p_hgs->rxBuffer[0] == p_hgs->deviceAddress) && (MB_checkCrc16Rx(p_hgs))) {
            switch (p_hgs->rxBuffer[1]) {
                case MB_FUNCTION_F3:
                    MB_f3(p_hgs);
                    break;
                case MB_FUNCTION_F6:
                    MB_f6(p_hgs);
                    break;
                case MB_FUNCTION_F16:
                    MB_f16(p_hgs);
                    break;
                default:
                    break;
            }
        }
        MB_rxBufferClear(p_hgs);
        MB_frameReset(p_hgs);
        MB_frameRead(p_hgs);
    }
}

bool MB_checkRegister(MB_bufferAndState_h *p_hbs, uint16_t address) {
    if (p_hbs[address].state != p_hbs[address].buffer) {
        p_hbs[address].state = p_hbs[address].buffer;
        return true;
    }
    return false;
}

uint16_t MB_getRegister(MB_bufferAndState_h *p_hbs, uint16_t address) {
    return p_hbs[address].state;
}

void MB_setRegister(MB_bufferAndState_h *p_hbs, uint16_t address, uint16_t data) {
    p_hbs[address].state = p_hbs[address].buffer = data;
}

static uint16_t MB_getCrc16(uint8_t *buf, uint16_t length) {
    static const uint16_t crcTable[] = {
            0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
            0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
            0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
            0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
            0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
            0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
            0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
            0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
            0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
            0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
            0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
            0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
            0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
            0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
            0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
            0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
            0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
            0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
            0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
            0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
            0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
            0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
            0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
            0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
            0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
            0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
            0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
            0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
            0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
            0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
            0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
            0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040};

    uint8_t nTemp;
    uint16_t crcWord = 0xFFFF;

    while (length--) {
        nTemp = *buf++ ^ crcWord;
        crcWord >>= 8;
        crcWord ^= crcTable[nTemp];
    }
    return crcWord;
}

static bool MB_checkCrc16Rx(MB_general_h *p_hgs) {
    bool status = false;
    uint16_t crcCalculate;
    uint16_t crcReceived;
    uint16_t length;

    switch (p_hgs->rxBuffer[1]) {
        case MB_FUNCTION_F3:
        case MB_FUNCTION_F6:
            length = 6; // Стандартный запрос: 1 байт адрес + 1 функция + 4 данные
            break;
        case MB_FUNCTION_F16:
            // Длина: 1(адрес) + 1(функция) + 2(нач.адр) + 2(кол-во) + 1(байт-счетчик) + сами данные
            length = 7 + p_hgs->rxBuffer[6];
            break;
        default:
            return false;
    }

    crcCalculate = MB_getCrc16(p_hgs->rxBuffer, length);
    crcReceived = U8D_TO_U16(p_hgs->rxBuffer[length + 1], p_hgs->rxBuffer[length]);
    if (crcCalculate == crcReceived) status = true;

    return status;
}

static bool MB_checkCrc16Len(MB_general_h *p_hgs, uint16_t length) {
    bool status = false;
    uint16_t crcCalculate;
    uint16_t crcReceived;

    crcCalculate = MB_getCrc16(p_hgs->rxBuffer, length);
    crcReceived = U8D_TO_U16(p_hgs->rxBuffer[length + 1], p_hgs->rxBuffer[length]);
    if (crcCalculate == crcReceived) status = true;
    return status;
}

static void MB_frameRead(MB_general_h *p_hgs) {
    MB_read(p_hgs, p_hgs->rxBuffer + p_hgs->byteCount, 1);
    p_hgs->byteCount++;
}

static void MB_frameReset(MB_general_h *p_hgs) {
    p_hgs->packetStatus = MB_P_RESET;
    p_hgs->byteCount = 0;
    p_hgs->timeGap = 0;
}

static void MB_rxBufferClear(MB_general_h *p_hgs) {
    for (int i = 0; i < MB_BUFFER_RW_SIZE_MAX; ++i) {
        p_hgs->rxBuffer[i] = 0;
    }
}

//Read Holding Registers
static void MB_f3(MB_general_h *p_hgs) {
    uint16_t beginRegisterAddress = U8D_TO_U16(p_hgs->rxBuffer[2], p_hgs->rxBuffer[3]);
    uint16_t registerCount = U8D_TO_U16(p_hgs->rxBuffer[4], p_hgs->rxBuffer[5]);
    uint16_t byteCount = 0;
    uint16_t index = 3;
    uint16_t crc;

    for (int i = 0; i < index; ++i) {
        p_hgs->txBuffer[i] = p_hgs->rxBuffer[i];
    }

    if (beginRegisterAddress + registerCount < MB_BUFFER_HOLDING_SIZE_MAX) {
        while (registerCount--) {
            p_hgs->txBuffer[index++] = U16_TO_U8H(p_hgs->buffer.holding[beginRegisterAddress].buffer);
            p_hgs->txBuffer[index++] = U16_TO_U8L(p_hgs->buffer.holding[beginRegisterAddress].buffer);
            ++beginRegisterAddress;
            byteCount += 2;
        }
        p_hgs->txBuffer[2] = byteCount;
        crc = MB_getCrc16(p_hgs->txBuffer, byteCount + 3);
        p_hgs->txBuffer[index++] = U16_TO_U8L(crc);
        p_hgs->txBuffer[index] = U16_TO_U8H(crc);
        MB_write(p_hgs, p_hgs->txBuffer, byteCount + 5);
    }
}

//Write Single Register
static void MB_f6(MB_general_h *p_hgs) {
    uint16_t registerAddress = U8D_TO_U16(p_hgs->rxBuffer[2], p_hgs->rxBuffer[3]);
    uint16_t data = U8D_TO_U16(p_hgs->rxBuffer[4], p_hgs->rxBuffer[5]);

    if (registerAddress < MB_BUFFER_HOLDING_SIZE_MAX) {
        p_hgs->buffer.holding[registerAddress].buffer = data;
        for (int i = 0; i < 8; ++i) {
            p_hgs->txBuffer[i] = p_hgs->rxBuffer[i];
        }
        MB_write(p_hgs, p_hgs->txBuffer, 8);
    }
}

// Write Multiple Registers (0x10)
static void MB_f16(MB_general_h *p_hgs) {
    uint16_t beginRegisterAddress = U8D_TO_U16(p_hgs->rxBuffer[2], p_hgs->rxBuffer[3]);
    uint16_t registerCount = U8D_TO_U16(p_hgs->rxBuffer[4], p_hgs->rxBuffer[5]);
    uint8_t byteCount = p_hgs->rxBuffer[6];
    uint16_t crc;

    // Проверка лимитов буфера и корректности количества байт
    if ((beginRegisterAddress + registerCount <= MB_BUFFER_HOLDING_SIZE_MAX) &&
        (byteCount == registerCount * 2)) {

        uint16_t dataIdx = 7;
        for (uint16_t i = 0; i < registerCount; i++) {
            p_hgs->buffer.holding[beginRegisterAddress + i].buffer =
                U8D_TO_U16(p_hgs->rxBuffer[dataIdx], p_hgs->rxBuffer[dataIdx + 1]);
            dataIdx += 2;
        }

        // Формируем ответ (Адрес, Функция, Нач. адрес, Кол-во регистров, CRC)
        for (int i = 0; i < 6; i++) {
            p_hgs->txBuffer[i] = p_hgs->rxBuffer[i];
        }

        crc = MB_getCrc16(p_hgs->txBuffer, 6);
        p_hgs->txBuffer[6] = U16_TO_U8L(crc);
        p_hgs->txBuffer[7] = U16_TO_U8H(crc);

        MB_write(p_hgs, p_hgs->txBuffer, 8);
        }
}

MB_status MB_f3Send(MB_general_h *p_hgs,
                    uint8_t slaveAddress,
                    uint16_t beginRegisterAddress,
                    uint16_t registerCount) {
    MB_status status = MB_ERROR;
    if (registerCount * 2 < MB_BUFFER_RW_SIZE_MAX) {
        uint8_t index = 0;
        uint8_t getByteCount;
        uint16_t crc;

        while (p_hgs->timeGap) {
        }

        p_hgs->txBuffer[index++] = slaveAddress;
        p_hgs->txBuffer[index++] = 0x03;

        p_hgs->txBuffer[index++] = U16_TO_U8H(beginRegisterAddress);
        p_hgs->txBuffer[index++] = U16_TO_U8L(beginRegisterAddress);

        p_hgs->txBuffer[index++] = U16_TO_U8H(registerCount);
        p_hgs->txBuffer[index++] = U16_TO_U8L(registerCount);

        crc = MB_getCrc16(p_hgs->txBuffer, index);
        p_hgs->txBuffer[index++] = U16_TO_U8L(crc);
        p_hgs->txBuffer[index++] = U16_TO_U8H(crc);

        MB_rxBufferClear(p_hgs);
        MB_frameReset(p_hgs);
        MB_frameRead(p_hgs);

        MB_write(p_hgs, p_hgs->txBuffer, index);

        p_hgs->timeOut = MB_PACKET_TIME_OUT;
        while ((p_hgs->packetStatus != MB_P_COMPLETE) && (p_hgs->timeOut != 0)) {}

        if (p_hgs->packetStatus == MB_P_COMPLETE) {
            getByteCount = p_hgs->rxBuffer[2];
            if (getByteCount == registerCount * 2) {
                if (MB_checkCrc16Len(p_hgs, getByteCount + 3)) {
                    status = MB_OK;
                }
            }
        }

        if (status == MB_OK) {
            uint8_t *p_bufH = &p_hgs->rxBuffer[3];
            uint8_t *p_bufL = &p_hgs->rxBuffer[4];
            while (registerCount--) {
                p_hgs->buffer.holding[beginRegisterAddress++].buffer = U8D_TO_U16(*p_bufH, *p_bufL);
                p_bufH += 2;
                p_bufL += 2;
            }
        }

        MB_rxBufferClear(p_hgs);
        MB_frameReset(p_hgs);
        MB_frameRead(p_hgs);

        p_hgs->timeGap = MB_PACKET_TIME_GAP + 1;
    }

    return status;
}

MB_status MB_f6Send(MB_general_h *p_hgs,
                    uint8_t slaveAddress,
                    uint16_t registerAddress,
                    uint16_t data) {
    MB_status status = MB_ERROR;
    uint8_t index = 0;
    uint16_t crc;

    while (p_hgs->timeGap) {
    }

    p_hgs->txBuffer[index++] = slaveAddress;
    p_hgs->txBuffer[index++] = 0x06;

    p_hgs->txBuffer[index++] = U16_TO_U8H(registerAddress);
    p_hgs->txBuffer[index++] = U16_TO_U8L(registerAddress);

    p_hgs->txBuffer[index++] = U16_TO_U8H(data);
    p_hgs->txBuffer[index++] = U16_TO_U8L(data);

    crc = MB_getCrc16(p_hgs->txBuffer, index);
    p_hgs->txBuffer[index++] = U16_TO_U8L(crc);
    p_hgs->txBuffer[index++] = U16_TO_U8H(crc);

    MB_rxBufferClear(p_hgs);
    MB_frameReset(p_hgs);
    MB_frameRead(p_hgs);

    MB_write(p_hgs, p_hgs->txBuffer, index);

    p_hgs->timeOut = MB_PACKET_TIME_OUT;
    while ((p_hgs->packetStatus != MB_P_COMPLETE) && (p_hgs->timeOut != 0)) {}

    if (p_hgs->packetStatus == MB_P_COMPLETE) {
        for (int i = 0; i < index; ++i) {
            if (p_hgs->txBuffer[i] != p_hgs->rxBuffer[i]) break;
            if (i == index - 1) status = MB_OK;
        }
    }

    MB_rxBufferClear(p_hgs);
    MB_frameReset(p_hgs);
    MB_frameRead(p_hgs);

    p_hgs->timeGap = MB_PACKET_TIME_GAP + 1;

    return status;
}

MB_status MB_f16Send(MB_general_h *p_hgs,
                    uint8_t slaveAddress,
                    uint16_t beginRegisterAddress,
                    uint16_t registerCount) {
    MB_status status = MB_ERROR;
    uint16_t byteCount = registerCount * 2;

    if (byteCount + 9 > MB_BUFFER_RW_SIZE_MAX) return MB_ERROR;

    while (p_hgs->timeGap) {}

    uint8_t index = 0;
    p_hgs->txBuffer[index++] = slaveAddress;
    p_hgs->txBuffer[index++] = 0x10; // Function F16
    p_hgs->txBuffer[index++] = U16_TO_U8H(beginRegisterAddress);
    p_hgs->txBuffer[index++] = U16_TO_U8L(beginRegisterAddress);
    p_hgs->txBuffer[index++] = U16_TO_U8H(registerCount);
    p_hgs->txBuffer[index++] = U16_TO_U8L(registerCount);
    p_hgs->txBuffer[index++] = (uint8_t)byteCount;

    // Копируем данные из внутреннего буфера в txBuffer
    for (uint16_t i = 0; i < registerCount; i++) {
        uint16_t val = p_hgs->buffer.holding[beginRegisterAddress + i].buffer;
        p_hgs->txBuffer[index++] = U16_TO_U8H(val);
        p_hgs->txBuffer[index++] = U16_TO_U8L(val);
    }

    uint16_t crc = MB_getCrc16(p_hgs->txBuffer, index);
    p_hgs->txBuffer[index++] = U16_TO_U8L(crc);
    p_hgs->txBuffer[index++] = U16_TO_U8H(crc);

    MB_rxBufferClear(p_hgs);
    MB_frameReset(p_hgs);
    MB_frameRead(p_hgs);

    MB_write(p_hgs, p_hgs->txBuffer, index);

    p_hgs->timeOut = MB_PACKET_TIME_OUT;
    while ((p_hgs->packetStatus != MB_P_COMPLETE) && (p_hgs->timeOut != 0)) {}

    if (p_hgs->packetStatus == MB_P_COMPLETE) {
        // Проверка эхо-ответа (первые 6 байт должны совпадать с запросом)
        if (MB_checkCrc16Len(p_hgs, 6)) {
            bool match = true;
            for (int i = 0; i < 6; i++) {
                if (p_hgs->rxBuffer[i] != p_hgs->txBuffer[i]) {
                    match = false;
                    break;
                }
            }
            if (match) status = MB_OK;
        }
    }

    MB_rxBufferClear(p_hgs);
    MB_frameReset(p_hgs);
    MB_frameRead(p_hgs);
    p_hgs->timeGap = MB_PACKET_TIME_GAP + 1;

    return status;
}
