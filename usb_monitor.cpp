#include "usb_monitor.h"
#include <QDebug>
#include <QThread>

UsbMonitor::UsbMonitor() {
    initDevicesCount_ = getUsbDeviceList().count();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &UsbMonitor::checkUsbDevices);
    timer->start(2000);
}

QStringList UsbMonitor::getUsbDeviceList() {
    QProcess process;
    process.start("lsusb");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QStringList dev_list = output.split("\n");
    dev_list.removeAll("");
    return dev_list;
}

void UsbMonitor::checkUsbDevices() {
    QStringList currentDevices = getUsbDeviceList();
    if (is_forced_upd_) {
        emit usbChanged(currentDevices);
        is_forced_upd_ = false;
    }
    if (currentDevices.count() != initDevicesCount_) {
        QProcess process;
        process.start("echo '0' | sudo tee /sys/devices/platform/soc/3f980000.usb/buspower");
        process.waitForFinished();
        QThread::msleep(1000);
        process.start("echo '1' | sudo tee /sys/devices/platform/soc/3f980000.usb/buspower");
        process.waitForFinished();
        emit usbChanged(currentDevices);
        is_forced_upd_ = true;
    }
}
