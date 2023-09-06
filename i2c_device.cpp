#include "i2c_device.h"

#include <boost/crc.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

Q_I2cDevice::Q_I2cDevice(QObject *parent)
    : QObject(parent)
    , st_msg_{ .crc = 0, .data = 0 } {
}

void Q_I2cDevice::init(int adapter_nr) {
    adapter_nr_ = adapter_nr;

    std::string filename = "/dev/i2c-" + std::to_string(adapter_nr_);
    handle_ = open(filename.c_str(), O_RDWR);

    debug_map_["handle"] = handle_;
    debug_map_["init_strerror"] = QString{ strerror(errno) };
    emit log(debug_map_);

    if (handle_ < 0) {
      exit(1);
    }
}

Q_I2cDevice::~Q_I2cDevice() {
    close(handle_);
}

QString Q_I2cDevice::send(int mcu, int data) {
    boost::crc_32_type crc32;

    crc32.reset();
    crc32.process_bytes(&st_msg_.data, sizeof(st_msg_.data));
    st_msg_.crc = crc32.checksum();
    st_msg_.data = data;

    debug_map_["crc"] = st_msg_.crc;

    /* Запись в slave */
    uint8_t write_buffer[MSG_SIZE];
    for(int i = 0; i < sizeof(st_msg_); ++i) {
        std::memcpy(write_buffer + i, &st_msg_, MSG_SIZE);
    }

    messages_[0].addr  = dev_address[mcu];
    messages_[0].flags = 0; // флаги записи
    messages_[0].len   = MSG_SIZE;
    messages_[0].buf   = write_buffer;

    /* Чтение из slave */
    uint8_t read_buffer[MSG_SIZE];
    messages_[1].addr  = dev_address[mcu];
    messages_[1].flags = I2C_M_RD; // флаги чтения
    messages_[1].len   = MSG_SIZE;
    messages_[1].buf   = read_buffer;

    /* Пакет для отправки */
    packets_.msgs      = messages_;
    packets_.nmsgs     = 2; // отправляем два сообщения

    int ret = ioctl(handle_, I2C_RDWR, &packets_);
    ++msg_cnt_;

    debug_map_["ioctl_return"] = ret;
    debug_map_["ioctl_strerror"] = QString{ strerror(errno) };
    QString hexString;
    for (int i = MSG_SIZE-1; i >= 0; --i) {
        hexString.append(QString::number(read_buffer[i], 16).rightJustified(2, '0'));
        hexString.append(" ");
    }
    debug_map_["read_buffer_"] = hexString.trimmed();
    debug_map_["msg_cnt"] = msg_cnt_;
    emit log(debug_map_);

    return {};
}

//void Q_I2cDevice::Serialize() {
//    for(int i = 0; i < sizeof(st_msg_); ++i) {
//        std::memcpy(write_buffer_ + i, &st_msg_, sizeof(st_msg_));
//    }
//}
