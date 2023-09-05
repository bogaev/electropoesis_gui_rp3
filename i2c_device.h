#pragma once

#include "enum_types.h"
#include <cstdint>
#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

#include <vector>

#include <boost/asio.hpp>

//class LogReceiver : public QObject {
//    Q_OBJECT

//public slots:
//    void receiveLog(QVariantMap logData) {
//        // Обработка данных logData
//    }
//};

class Q_I2cDevice : public QObject {
    Q_OBJECT

public:
    explicit Q_I2cDevice(QObject *parent = nullptr);
    Q_INVOKABLE void init(boost::asio::posix::stream_descriptor& stream_descr,
                          int adapter_nr = 1);
    Q_INVOKABLE QString send(int mcu, int data);
    void read();
    ~Q_I2cDevice();

signals:
    void commandSent(QString result);
    void log(QVariantMap debug_map);

private:
    struct I2cMcuAddr {
      constexpr static uint8_t MAIN = 0xAA;
      constexpr static uint8_t ADD = 0x55;
    };

    void onWrite(const boost::system::error_code& error, size_t bytes_transferred);
    void onRead(const boost::system::error_code& error, size_t bytes_transferred);

    boost::asio::posix::stream_descriptor* stream_descr_;
    std::function<void(QString)> callback_{};

    int adapter_nr_ = 1;
    std::vector<uint8_t> read_buffer_;
    int handle_ = -1;
    uint8_t dev_address[2] =
        { I2cMcuAddr::MAIN,
          I2cMcuAddr::ADD   };

    QVariantMap debug_map_;
    int msg_cnt_ = 0;
};
