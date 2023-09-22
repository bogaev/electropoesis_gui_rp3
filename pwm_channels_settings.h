#pragma once

#include "enum_types.h"
#include <cstdint>
#include <vector>
#include <QObject>
#include <QString>
#include <QCheckBox>
#include <fstream>

#include "i2c_device.h"


class PwmChannelsSettings : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool qSingleSelect WRITE setSingleSelect)

    struct ChannelParams {
        quint8 chbx_ = 0;
        quint8 type_ = 0;
        quint8 amp_ = 0;
        quint8 res1_ = 0xFF;
        quint16 freq_ = 0;
        quint16 res2_ = 0xFFFF;

        ChannelParams() = default;
        ChannelParams(quint8 chbx, quint8 type, quint8 amp, quint16 freq)
            : chbx_(chbx), type_(type), amp_(amp), freq_(freq) {}
    };

public:
    void setSingleSelect(bool single) {
        singleSelect = single;
    }

    explicit PwmChannelsSettings(QObject *parent = nullptr);
    void setI2cDevice(Q_I2cDevice& i2c);

    Q_INVOKABLE void setCheckboxStatus(int index, Qt::CheckState status);
    Q_INVOKABLE void setAllChannels(Qt::CheckState value);
    Q_INVOKABLE void setSignalParams(quint8 signal_id,
                                     quint8 type,
                                     quint8 amp,
                                     quint16 freq);
    Q_INVOKABLE void commitChanges();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void relayOn();
    Q_INVOKABLE quint8 getStatus(quint8 mcu);
    Q_INVOKABLE void initAllChannels();

signals:
    void checkboxChanged(int index, int status);
    void singleCheckboxChanged(int index);
    void uartStatusChanged(const QString &newStatus);
    void isNonCosCarrier(bool value);
    void signalParamChanged(quint8 ch, quint8 chbx, quint8 sig, quint8 type, quint8 amp, quint16 freq);

private:
    enComStatus checkMcuStatus();
    void sendAllIfOk(tdMessageType command);

    bool singleSelect = Qt::Unchecked;
    Qt::CheckState isChannelSelected[CHANNELS_NUM];
    Q_I2cDevice* i2c_;
    std::ofstream outFile_;

    enComStatus mcu_main_ = COM_STATUS_ERR;
    enComStatus mcu_add_ = COM_STATUS_ERR;

    enMcuStatus mcu_main_status_ = MCU_STATUS_RESTARTED;
    enMcuStatus mcu_add_status_ = MCU_STATUS_RESTARTED;

    template<typename Predicate>
    void setAllCheckboxesIf(Qt::CheckState value, Predicate comp);

    void saveToFile(quint8 ch, quint8 signal_id, const ChannelParams& params);
    bool loadFromFile(quint8 ch, quint8 signal_id, ChannelParams& params);
};

template <typename Predicate>
void PwmChannelsSettings::setAllCheckboxesIf(Qt::CheckState value, Predicate comp) {
    for(int i = 0; i < CHANNELS_NUM; ++i) {
        if(comp(i)) {
            isChannelSelected[i] = value;
            emit checkboxChanged(i, isChannelSelected[i]);
        }
    }
}
