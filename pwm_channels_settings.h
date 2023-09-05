#pragma once

#include "enum_types.h"
#include <cstdint>
#include <vector>
#include <QObject>
#include <QString>
#include <QCheckBox>

#include "i2c_device.h"

struct tdSignal {
    tdSignalTypes type = SIGNAL_TYPE_NONE;
    uint16_t amp = 1.f;
    uint8_t freq = 16U;
};

struct tdEmitter {
    std::vector<tdSignal> signalParams{UART_SIGNAL_NUM};
};

class PwmChannelsSettings : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool qSingleSelect WRITE setSingleSelect)

public:
    void setSingleSelect(bool single) {
        singleSelect = single;
    }

    explicit PwmChannelsSettings(QObject *parent = nullptr);
    void setI2cDevice(Q_I2cDevice& i2c);

    Q_INVOKABLE void setCheckboxStatus(int index, Qt::CheckState status);
    Q_INVOKABLE void setAllChannels(Qt::CheckState value);
    Q_INVOKABLE void setSignalParams(int signal_id,
                                     int type,
                                     QString amp,
                                     QString freq);
    Q_INVOKABLE void commitChanges();
    Q_INVOKABLE void comTest();

signals:
    void checkboxChanged(int index, int status);
    void singleCheckboxChanged(int index);
    void uartStatusChanged(const QString &newStatus);
    void isNonCosCarrier(bool value);

private:
    bool singleSelect = Qt::Unchecked;
    Qt::CheckState isEmitterSelected[EMITTERS_NUM];
    std::vector<tdEmitter> emitters{EMITTERS_NUM};
    Q_I2cDevice* i2c_;

    template<typename Predicate>
    void setAllCheckboxesIf(Qt::CheckState value, Predicate comp);
};

template <typename Predicate>
void PwmChannelsSettings::setAllCheckboxesIf(Qt::CheckState value, Predicate comp) {
    for(int i = 0; i < EMITTERS_NUM; ++i) {
        if(comp(i)) {
            isEmitterSelected[i] = value;
            emit checkboxChanged(i, isEmitterSelected[i]);
        }
    }
}
