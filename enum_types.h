#pragma once

#include <cstdint>

constexpr int EMITTERS_NUM = 8;

typedef enum {
  UART_MESSAGE_DATA = 0,
  UART_MESSAGE_START,
  UART_MESSAGE_END
} tdMessageType;

typedef enum {
  UART_SIGNAL_CARRIER = 0,
  UART_SIGNAL_AMP_MOD,
  UART_SIGNAL_FREQ_MOD,
  UART_SIGNAL_NUM
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
  UART_PARAM_SIGNAL_TYPE = 0,
  UART_PARAM_AMP,
  UART_PARAM_FREQ,
  UART_PARAM_NUM
} enSignalParams;

typedef struct {
  uint8_t type = UART_MESSAGE_DATA;
  uint8_t emitter = 0;
  uint8_t signal = 0;
  uint8_t param = 0;
  uint16_t value = 0;
} tdPwmData;

typedef enum {
  EMCU_MAIN = 0,
  EMCU_ADD
} EMcu;

typedef struct {
  int crc;
  //tdPwmData data;
  int data;
} tdSTMessage;
