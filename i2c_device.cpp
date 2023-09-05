#include "i2c_device.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <linux/i2c-dev.h>

#include <boost/crc.hpp>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

Q_I2cDevice::Q_I2cDevice(QObject *parent)
        : QObject(parent)
        , read_buffer_(1024) {
}

void Q_I2cDevice::init(boost::asio::posix::stream_descriptor& stream_descr, int adapter_nr) {
    stream_descr_ = &stream_descr;
    adapter_nr_ = adapter_nr;

    std::string filename = "/dev/i2c-" + std::to_string(adapter_nr_);
    handle_ = open(filename.c_str(), O_RDWR);

    debug_map_["handle"] = handle_;
    debug_map_["init_strerror"] = QString{ strerror(errno) };
    emit log(debug_map_);

    if (handle_ < 0) {
      //std::cerr << "Error: " << strerror(errno) << std::endl;
      exit(1);
    }

    try {
        stream_descr_->assign(handle_);
    } catch (const boost::system::system_error& e) {
        debug_map_["stream_descr_err"] = QString::fromStdString(e.what());
        emit log(debug_map_);
    }
}

Q_I2cDevice::~Q_I2cDevice() {
    stream_descr_->close();
    close(handle_);
}

QString Q_I2cDevice::send(int mcu, int data) {
    boost::crc_32_type crc32;
    ++msg_cnt_;

    tdSTMessage msg{ .crc = 0, .data = data };
    crc32.reset();
    crc32.process_bytes(&msg.data, sizeof(msg.data));
    msg.crc = crc32.checksum();
    debug_map_["crc"] = msg.crc;

    int ret = ioctl(handle_, I2C_SLAVE, dev_address[mcu]);
    debug_map_["ioctl_return"] = ret;
    debug_map_["ioctl_strerror"] = QString{ strerror(errno) };
    debug_map_["msg_cnt"] = msg_cnt_;
    emit log(debug_map_);

    if (ret < 0) {
      exit(1);
    }

    int buf[10];
    buf[0] = msg.crc;
    buf[1] = msg.data;
//    ret = write(handle_, buf, sizeof(msg));
//    debug_map_["write_return"] = ret;
//    debug_map_["write_strerror"] = QString{ strerror(errno) };
//    emit log(debug_map_);
//    if (ret != 1) {
//      /* ERROR HANDLING: i2c transaction failed */
//    }

//    boost::asio::async_write( *stream_descr_, boost::asio::buffer(buf, sizeof(msg)),
//        std::bind(&Q_I2cDevice::onWrite, this, std::placeholders::_1, std::placeholders::_2) );

    boost::asio::async_write( *stream_descr_, boost::asio::buffer(buf, sizeof(msg)),
                  [this](const boost::system::error_code& error, size_t bytes_transferred) {
                      if (error) {
                          debug_map_["on_write_status"] = "Failed";
                          debug_map_["on_write_error"] = QString::fromStdString(error.message());
                      } else {
                          debug_map_["on_write_status"] = "Success";
                          debug_map_["on_bytes_transferred"] = static_cast<int>(bytes_transferred);
                      }
                      emit log(debug_map_);
                      read();
                  } );

    return {};
}

void Q_I2cDevice::onWrite(const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
        debug_map_["on_write_status"] = "Failed";
        debug_map_["on_write_error"] = QString::fromStdString(error.message());
    } else {
        debug_map_["on_write_status"] = "Success";
        debug_map_["on_bytes_transferred"] = static_cast<int>(bytes_transferred);
    }
    emit log(debug_map_);

    if (error) {
        // Handle the error...
        //callback_(QString::fromStdString(error.message()));
    } else {
        // Successfully wrote the data...
        //callback_(QString());
//        emit commandSent(QString()); // Emit signal to inform of the result
    }
}

void Q_I2cDevice::read() {
    boost::asio::async_read(*stream_descr_, boost::asio::buffer(read_buffer_),
          std::bind(&Q_I2cDevice::onRead, this, std::placeholders::_1, std::placeholders::_2));
}

void Q_I2cDevice::onRead(const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
        debug_map_["on_read_status"] = "Failed";
        debug_map_["on_read_error"] = QString::fromStdString(error.message());
    } else {
        debug_map_["on_read_status"] = "Success";
        debug_map_["on_bytes_transferred"] = static_cast<int>(bytes_transferred);
    }
    emit log(debug_map_);
}
