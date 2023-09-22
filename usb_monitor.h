#ifndef USBMONITOR_H
#define USBMONITOR_H

#include <QObject>
#include <QTimer>
#include <QProcess>

class UsbMonitor : public QObject {
    Q_OBJECT

public:
    UsbMonitor();

signals:
    void usbChanged(QStringList devicesList);

public slots:
    void checkUsbDevices();

private:
    QStringList getUsbDeviceList();
    int initDevicesCount_;
    bool is_forced_upd_ = true;
};

#endif // USBMONITOR_H
