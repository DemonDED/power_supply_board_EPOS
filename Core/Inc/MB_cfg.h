//
// Created by ded on 4/23/26.
// From alekseev/IST_DB_STM (Created by Alekseev on 27.10.2021)


#ifndef TEST_STM32H743_MB_CFG_H
#define TEST_STM32H743_MB_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MB_BUFFER_SIZE_MAX 20
// #define MB_BUFFER_COIL_SIZE_MAX             MB_BUFFER_SIZE_MAX
// #define MB_BUFFER_DISCRETE_SIZE_MAX         MB_BUFFER_SIZE_MAX
#define MB_BUFFER_HOLDING_SIZE_MAX MB_BUFFER_SIZE_MAX
// #define MB_BUFFER_INPUT_SIZE_MAX            MB_BUFFER_SIZE_MAX
#define MB_PACKET_TIME_OUT 200
#define MB_PACKET_TIME_GAP 1

#define MB_UTF_RESOLUTION (0.01f)
#define MB_FTU_RESOLUTION (100)

typedef union MB_REG_STATUS {
  uint16_t all;
  struct {
    uint16_t RUN : 1;
    uint16_t CV : 1;
    uint16_t CC : 1;
    uint16_t RES04 : 1;
    uint16_t RES05 : 1;
    uint16_t RES06 : 1;
    uint16_t RES07 : 1;
    uint16_t RES08 : 1;
    uint16_t RES09 : 1;
    uint16_t RES10 : 1;
    uint16_t RES11 : 1;
    uint16_t RES12 : 1;
    uint16_t RES13 : 1;
    uint16_t RES14 : 1;
    uint16_t RES15 : 1;
    uint16_t RES16 : 1;
  } bit;
} MB_REG_STATUS_STRUCT;

typedef union MB_REG_ERRDRV {
  uint16_t all;
  struct {
    uint16_t FAULT_1 : 1;
    uint16_t READY_1 : 1;
    uint16_t FAULT_2 : 1;
    uint16_t READY_2 : 1;
    uint16_t FAULT_3 : 1;
    uint16_t READY_3 : 1;
    uint16_t FAULT_4 : 1;
    uint16_t READY_4 : 1;
    uint16_t FAULT_5 : 1;
    uint16_t READY_5 : 1;
    uint16_t FAULT_6 : 1;
    uint16_t READY_6 : 1;
    uint16_t FAULT_7 : 1;
    uint16_t READY_7 : 1;
    uint16_t FAULT_8 : 1;
    uint16_t READY_8 : 1;
  } bit;
} MB_REG_ERRDRV_STRUCT;

typedef union MB_REG_ERR {
  uint16_t all;
  struct {
    uint16_t LOOP : 1;
    uint16_t BRIDGE : 1;
    uint16_t CUR_CH1 : 1;
    uint16_t CUR_CH2 : 1;
    uint16_t CUR_BR : 1;
    uint16_t RESERV_11 : 1;
    uint16_t RESERV_10 : 1;
    uint16_t RESERV_09 : 1;
    uint16_t RESERV_08 : 1;
    uint16_t RESERV_07 : 1;
    uint16_t RESERV_06 : 1;
    uint16_t RESERV_05 : 1;
    uint16_t RESERV_04 : 1;
    uint16_t RESERV_03 : 1;
    uint16_t RESERV_02 : 1;
    uint16_t RESERV_01 : 1;
  } bit;
} MB_REG_ERR_STRUCT;

typedef struct {
  MB_REG_STATUS_STRUCT status;
  MB_REG_ERRDRV_STRUCT errDrv;
  union MB_REG_ERR err;
} MB_ERROR_TypeDef;

typedef enum {
  MB_REG_VERSION = 0,

  MB_REG_ONOFF,
  MB_REG_STATUS,
  MB_REG_ERROR_DRIVER,
  MB_REG_ERROR,

  MB_REG_SET_FREQ,     ///< from 1 to 100% PERIOD_BRIDGE_MIN/MAX
  MB_REG_SET_CURRENT,  ///< set current resolution 0.01
  MB_REG_GET_CURRENT,  ///< get current feedback resolution 0.01

  MB_REG_DEBUG,

  MB_REG_TOTAL,
} MB_holdingAddress;

#ifdef __cplusplus
}
#endif

#endif //TEST_STM32H743_MB_CFG_H
