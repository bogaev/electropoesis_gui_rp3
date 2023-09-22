#include <QQuickView>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>

#include <boost/asio.hpp>

#include "i2c_device.h"
#include "pwm_channels_settings.h"
#include "tablemodel.h"
#include "usb_monitor.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    qmlRegisterType<TableModel>("TableModel", 0, 1, "TableModel");
    qmlRegisterType(QUrl("qrc:DebugPanel.qml"), "Debug", 1, 0, "DebugPanel");
    qmlRegisterType<UsbMonitor>("UsbTools", 1, 0, "UsbMonitor");

    Q_I2cDevice i2c_device;
    PwmChannelsSettings pwm_settings;
    pwm_settings.setI2cDevice(i2c_device);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("i2c_device", &i2c_device);
    engine.rootContext()->setContextProperty("pwm_settings", &pwm_settings);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    i2c_device.init();

    int app_exit_code = app.exec();

    return app_exit_code;
}
