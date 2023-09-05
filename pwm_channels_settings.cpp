#include "pwm_channels_settings.h"
#include <QThread>

#define PAUSE_MS 2U

PwmChannelsSettings::PwmChannelsSettings(QObject *parent)
    : QObject(parent)
    //        uart(parent)
{
    isEmitterSelected[0] = Qt::Checked;
    singleSelect = true;
//    uart.init();
}

void PwmChannelsSettings::setI2cDevice(Q_I2cDevice& i2c) {
    i2c_ = &i2c;
    // Additional initialization code
}

void PwmChannelsSettings::setCheckboxStatus(int index, Qt::CheckState status) {
    isEmitterSelected[index] = status;
    if(singleSelect) {
        setAllCheckboxesIf(Qt::Unchecked, [index](int i){return i != index;});
        emit singleCheckboxChanged(index);
    }
}

void PwmChannelsSettings::setAllChannels(Qt::CheckState value) {
    setAllCheckboxesIf(value, [](int){return true;});
}

void PwmChannelsSettings::setSignalParams(int signal_id, int type, QString amp, QString freq) {
    QString debugMessage;
    if (signal_id == UART_SIGNAL_CARRIER && type != SIGNAL_TYPE_SINUS) {
        emit isNonCosCarrier(true);
    } else {
        emit isNonCosCarrier(false);
    }
//    uart.sendCommand(0, UART_MESSAGE_START);
    //QThread::msleep(200);
    for (auto i = 0; i < EMITTERS_NUM; ++i) {
        if (isEmitterSelected[i] == Qt::Checked) {
            //emitters[i].signalParams[signal_id].type = (tdSignalTypes)type;
            //debugMessage += uart.sendCommand(i, signal_id, UART_PARAM_SIGNAL_TYPE, (uint8_t)type);
            i2c_->send(EMCU_MAIN, 0xDEADBEEF);
            //QThread::msleep(PAUSE_MS);
            //emitters[i].signalParams[signal_id].amp = amp.toInt();
            //debugMessage += uart.sendCommand(i, signal_id, UART_PARAM_AMP, (uint16_t)amp.toInt());
            //QThread::msleep(PAUSE_MS);
            //emitters[i].signalParams[signal_id].freq = freq.toInt();
            //debugMessage += uart.sendCommand(i, signal_id, UART_PARAM_FREQ, (uint8_t)freq.toInt());
            //QThread::msleep(PAUSE_MS);
            //debugMessage += uart.sendCommand(i, UART_MESSAGE_END);
            //QThread::msleep(PAUSE_MS);
        }
        emit uartStatusChanged(debugMessage);
    }
}

void PwmChannelsSettings::commitChanges() {
    for (auto i = 0; i < EMITTERS_NUM; ++i) {
        if (isEmitterSelected[i] == Qt::Checked) {
            //uart.sendCommand(i, UART_MESSAGE_END);
            QThread::msleep(PAUSE_MS);
        }
    }
}

void PwmChannelsSettings::comTest() {
    i2c_->send(EMCU_MAIN, 0xDEADBEEF);
//    QString debugMessage;
//    debugMessage += uart.init();
//    debugMessage += uart.sendCommand();
//    emit uartStatusChanged(debugMessage);
}
