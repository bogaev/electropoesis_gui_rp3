#pragma once

#include "enum_types.h"

#include <cstdint>

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

#include <vector>
#include <utility>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

constexpr uint8_t MAIN_ADDR = 0xAA;
constexpr uint8_t ADD_ADDR = 0x55;
struct McuAnswer {
    uint8_t com_status;
    uint8_t mcu_status;
};

class Q_I2cDevice : public QObject {
    Q_OBJECT

public:
    explicit Q_I2cDevice(QObject *parent = nullptr);
    Q_INVOKABLE void init(int adapter_nr = 1);
    McuAnswer send(quint32 ch, tdPwmData data);
    McuAnswer send(uint8_t addr, tdMessageType msg_type);
    ~Q_I2cDevice();

signals:
    void log(QVariantMap debug_map);

private:
    constexpr static uint8_t MSG_SIZE = sizeof(tdSTMessage);
    constexpr static uint8_t DATA_SIZE = sizeof(tdPwmData);
    constexpr static uint8_t ANSWER_SIZE = 2;
    constexpr static uint8_t MAX_REPEATS = 10;
    constexpr static uint8_t CH_TO_I2C_ADDR[CHANNELS_NUM] = {
        MAIN_ADDR,
        MAIN_ADDR,
        ADD_ADDR,
        MAIN_ADDR,
        ADD_ADDR,
        MAIN_ADDR,
        ADD_ADDR,
        ADD_ADDR
    };

    int handle_ = -1;
    int adapter_nr_ = 1;

    struct i2c_rdwr_ioctl_data packets_;
    struct i2c_msg messages_[2];

    tdSTMessage st_msg_{};

    QVariantMap debug_map_;
    int msg_cnt_ = 0;
    int err_cnt_ = 0;
};
