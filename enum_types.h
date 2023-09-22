#pragma once

#include <cstdint>

constexpr int CHANNELS_NUM = 8;
constexpr int SIGNALS_NUM = 3;

typedef enum {
    COM_MSG_NONE = 0,
    COM_MSG_CHANGE_PARAM,
    COM_MSG_COMMIT,
    COM_MSG_CHECK_MCU_STATUS,
    COM_MSG_RELAY_ON,
    COM_MSG_RESUME,
    COM_MSG_PAUSE
} tdMessageType;

typedef enum {
  COM_SIGNAL_CARRIER = 0,
  COM_SIGNAL_AMP_MOD,
  COM_SIGNAL_FREQ_MOD,
  COM_SIGNAL_NUM
} enPwmSignals;

typedef enum {
  SIGNAL_TYPE_NONE = 0,
  SIGNAL_TYPE_SINUS,
  SIGNAL_TYPE_MEANDR,
  SIGNAL_TYPE_TRIANGLE,
  SIGNAL_TYPE_SAW,
  SIGNAL_TYPE_NUM
} tdSignalTypes;

typedef enum {
  COM_PARAM_SIGNAL_TYPE = 0,
  COM_PARAM_AMP,
  COM_PARAM_FREQ,
  COM_PARAM_NUM
} enSignalParams;

typedef struct {
    uint32_t type:4;
    uint32_t emitter:4;
    uint32_t signal:4;
    uint32_t param:4;
    uint32_t value:16;
} tdPwmData;

typedef enum {
    COM_STATUS_OK = 0,
    COM_STATUS_CRC_ERR,
    COM_STATUS_OS_ERR,
    COM_STATUS_ERR
} enComStatus;

typedef enum {
    MCU_STATUS_RESTARTED = 0,
    MCU_STATUS_RELAY_ON,
    MCU_STATUS_RESUME,
    MCU_STATUS_PAUSE,
    MCU_STATUS_ERR
} enMcuStatus;

typedef struct {
    uint32_t crc;
    tdPwmData data;
} tdSTMessage;
