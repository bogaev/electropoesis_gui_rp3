#pragma once

#include "enum_types.h"

#include <cstdint>

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

class Q_I2cDevice : public QObject {
    Q_OBJECT

public:
    explicit Q_I2cDevice(QObject *parent = nullptr);
    Q_INVOKABLE void init(int adapter_nr = 1);
    Q_INVOKABLE QString send(int mcu, int data);
    ~Q_I2cDevice();

signals:
    void log(QVariantMap debug_map);

private:    
    constexpr static uint8_t MSG_SIZE = sizeof(tdSTMessage);
    constexpr static uint8_t DATA_SIZE = sizeof(tdSTMessage::data);

    struct I2cMcuAddr {
      constexpr static uint8_t MAIN = 0xAA;
      constexpr static uint8_t ADD = 0x55;
    };

    uint8_t dev_address[2] =
        { I2cMcuAddr::MAIN,
          I2cMcuAddr::ADD   };

    int handle_ = -1;
    int adapter_nr_ = 1;

    struct i2c_rdwr_ioctl_data packets_;
    struct i2c_msg messages_[2];

    tdSTMessage st_msg_;

    QVariantMap debug_map_;
    int msg_cnt_ = 0;

//    void Serialize();
};
