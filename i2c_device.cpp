#include "i2c_device.h"

#include <boost/crc.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

constexpr uint8_t Q_I2cDevice::CH_TO_I2C_ADDR[CHANNELS_NUM];

Q_I2cDevice::Q_I2cDevice(QObject *parent)
    : QObject(parent) {
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

McuAnswer Q_I2cDevice::send(quint32 ch, tdPwmData data) {
    st_msg_.data = data;

    boost::crc_32_type crc32;
    crc32.reset();
    crc32.process_bytes(&st_msg_.data, DATA_SIZE);
    st_msg_.crc = crc32.checksum();
    debug_map_["crc"] = st_msg_.crc;

    /* Запись в slave */
    uint8_t write_buffer[MSG_SIZE];
    std::memcpy(write_buffer, &st_msg_, MSG_SIZE);

    messages_[0].addr  = CH_TO_I2C_ADDR[ch];
    messages_[0].flags = 0; // флаги записи
    messages_[0].len   = MSG_SIZE;
    messages_[0].buf   = write_buffer;

    /* Чтение из slave */
    uint8_t read_buffer[ANSWER_SIZE];
    messages_[1].addr  = CH_TO_I2C_ADDR[ch];
    messages_[1].flags = I2C_M_RD; // флаги чтения
    messages_[1].len   = ANSWER_SIZE;
    messages_[1].buf   = read_buffer;

    /* Пакет для отправки */
    packets_.msgs      = messages_;
    packets_.nmsgs     = 2;

    int ret = -1;
    int repeat = 0;
    int com_status = -1;
    int mcu_status = -1;

    do {
        ret = ioctl(handle_, I2C_RDWR, &packets_);
        ++repeat;
        ++msg_cnt_;
        com_status = read_buffer[0];
        mcu_status = read_buffer[1];
        if (ret < 0 || com_status != COM_STATUS_OK) {
            ++err_cnt_;
        }
    } while ((ret < 0 || com_status != COM_STATUS_OK) && repeat < MAX_REPEATS);

    debug_map_["ioctl_return"] = ret;
    debug_map_["ioctl_strerror"] = QString{ strerror(errno) };
    debug_map_["Com status"] = com_status;
    debug_map_["MCU status"] = mcu_status;
    debug_map_["msg_cnt"] = msg_cnt_;
    debug_map_["err_cnt"] = err_cnt_;

    emit log(debug_map_);

    return McuAnswer {com_status, mcu_status};
}

McuAnswer Q_I2cDevice::send(uint8_t addr, tdMessageType msg_type) {
    McuAnswer answer;

    if (addr == MAIN_ADDR) {
        answer = send(0, tdPwmData { msg_type, 0, 0, 0, 0 });
        debug_map_["Main MCU com answer"] = int(answer.com_status);
        debug_map_["Main MCU answer"] = int(answer.mcu_status);
        emit log(debug_map_);
    } else if (addr == ADD_ADDR) {
        answer = send(2, tdPwmData { msg_type, 0, 0, 0, 0 });
        debug_map_["Add MCU com answer"] = int(answer.com_status);
        debug_map_["Add MCU answer"] = int(answer.mcu_status);
        emit log(debug_map_);
    }

    return answer;
}
