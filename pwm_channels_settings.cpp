#include "pwm_channels_settings.h"
#include <QThread>

#define PAUSE_MS 5U

PwmChannelsSettings::PwmChannelsSettings(QObject *parent)
    : QObject(parent) {
    singleSelect = true;
}

void PwmChannelsSettings::setI2cDevice(Q_I2cDevice& i2c) {
    i2c_ = &i2c;
}

void PwmChannelsSettings::setCheckboxStatus(int index, Qt::CheckState status) {
    isChannelSelected[index] = status;
    if(singleSelect) {
        setAllCheckboxesIf(Qt::Unchecked, [index](int i){return i != index;});
        emit singleCheckboxChanged(index);
    }
}

void PwmChannelsSettings::setAllChannels(Qt::CheckState value) {
    setAllCheckboxesIf(value, [](int){return true;});
}

void PwmChannelsSettings::setSignalParams(quint8 signal_id, quint8 type, quint8 amp, quint16 freq) {
    tdPwmData data{};

    if (signal_id == COM_SIGNAL_CARRIER && type != SIGNAL_TYPE_SINUS) {
        emit isNonCosCarrier(true);
    } else {
        emit isNonCosCarrier(false);
    }

    for (quint8 i = 0; i < CHANNELS_NUM; ++i) {
        bool chbx = (bool)isChannelSelected[i];
        saveToFile(i, signal_id, {(quint8)chbx, type, amp, freq});
        if (isChannelSelected[i] == Qt::Checked) {
            data = { COM_MSG_CHANGE_PARAM, i, signal_id, COM_PARAM_SIGNAL_TYPE, type };
            i2c_->send(i, data);
            data = { COM_MSG_CHANGE_PARAM, i, signal_id, COM_PARAM_AMP, amp };
            i2c_->send(i, data);
            data = { COM_MSG_CHANGE_PARAM, i, signal_id, COM_PARAM_FREQ, freq };
            i2c_->send(i, data);
        }
    }
}

void PwmChannelsSettings::commitChanges() {
    for (quint8 i = 0; i < CHANNELS_NUM; ++i) {
        if (isChannelSelected[i] == Qt::Checked) {
            tdPwmData data{ COM_MSG_COMMIT, i, 0, 0, 0 };
            i2c_->send(i, data);
//            QThread::msleep(PAUSE_MS);
        }
    }
}

enComStatus PwmChannelsSettings::checkMcuStatus() {
    auto main = i2c_->send(MAIN_ADDR, COM_MSG_CHECK_MCU_STATUS);
    auto add = i2c_->send(ADD_ADDR, COM_MSG_CHECK_MCU_STATUS);
    if (!main.com_status && !add.com_status) {
        return COM_STATUS_OK;
    }
    return COM_STATUS_ERR;
}

void PwmChannelsSettings::sendAllIfOk(tdMessageType command) {
    if (checkMcuStatus() == COM_STATUS_OK) {
        i2c_->send(MAIN_ADDR, command);
        i2c_->send(ADD_ADDR, command);
    }
}

void PwmChannelsSettings::relayOn() {
    sendAllIfOk(COM_MSG_RELAY_ON);
}

void PwmChannelsSettings::start() {
    sendAllIfOk(COM_MSG_RESUME);
}

void PwmChannelsSettings::stop() {
    sendAllIfOk(COM_MSG_PAUSE);
}

quint8 PwmChannelsSettings::getStatus(quint8 mcu) {
    auto mcu_answer = i2c_->send(mcu, COM_MSG_CHECK_MCU_STATUS);
    return mcu_answer.mcu_status;
}

void PwmChannelsSettings::saveToFile(quint8 ch, quint8 signal_id, const ChannelParams& params) {
    std::ofstream outFile("params.txt", std::ios::binary | std::ios::in | std::ios::out);
    if (outFile.is_open()) {
        long position = (ch * SIGNALS_NUM + signal_id) * sizeof(ChannelParams);
        outFile.seekp(position);
        outFile.write(reinterpret_cast<const char*>(&params), sizeof(ChannelParams));
        outFile.close();
    }
}

bool PwmChannelsSettings::loadFromFile(quint8 ch, quint8 signal_id, ChannelParams& params) {
    std::ifstream inFile("params.txt", std::ios::binary | std::ios::in);
    if(inFile.is_open()) {
        long position = (ch * SIGNALS_NUM + signal_id) * sizeof(ChannelParams);
        inFile.seekg(position);
        inFile.read(reinterpret_cast<char*>(&params), sizeof(ChannelParams));
        inFile.close();
    }
    return true;
}

void PwmChannelsSettings::initAllChannels() {
    ChannelParams p;
    for (quint8 i = 0; i < CHANNELS_NUM; ++i) {
        for (quint8 sig = 0; sig < SIGNALS_NUM; ++sig) {
            loadFromFile(i, sig, p);
            isChannelSelected[i] = p.chbx_ ? Qt::Checked : Qt::Unchecked;
            emit signalParamChanged(i, p.chbx_, sig, p.type_, p.amp_, p.freq_);
        }
    }
}
