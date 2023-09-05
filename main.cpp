#include <QQuickView>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>

#include <boost/asio.hpp>

#include "i2c_device.h"
#include "pwm_channels_settings.h"
#include "tablemodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    qmlRegisterType<PwmChannelsSettings>("PwmSettings", 0, 1, "PwmSettings");
    qmlRegisterType<TableModel>("TableModel", 0, 1, "TableModel");
    qmlRegisterType(QUrl("qrc:DebugPanel.qml"), "Debug", 1, 0, "DebugPanel");

    boost::asio::io_context io_context;
    boost::asio::posix::stream_descriptor stream_descr{io_context};
    Q_I2cDevice i2c_device;
//    LogReceiver logReceiver;

//    // Установка соединения
//    QObject::connect(&i2c_device, SIGNAL(log(QVariantMap)),
//            &logReceiver, SLOT(receiveLog(QVariantMap)),a
//            Qt::QueuedConnection);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("i2c_device", &i2c_device);

    auto* pwmSettings = engine.rootContext()->contextProperty("PwmSettings").value<PwmChannelsSettings*>();
//    pwmSettings->setI2cDevice(i2c);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    i2c_device.init(stream_descr);
    std::thread io_thread([&io_context]() {
        io_context.run();
    });

    int app_exit_code = app.exec();

    // Stop the io_context and join the thread before exiting
    io_context.stop();
    io_thread.join();

    return app_exit_code;
}
