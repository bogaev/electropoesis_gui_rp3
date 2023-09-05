#QT += quickcontrols2 qml

CONFIG += disable-desktop c++11 qmltypes static

#QML_IMPORT_NAME = io.qt.examples.backend SimpleRng
QML_IMPORT_NAME = SimpleRng
QML_IMPORT_MAJOR_VERSION = 1

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        i2c_device.cpp \
        main.cpp \
        pwm_channels_settings.cpp \
        tablemodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/usr/qt5-projects/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += qtquickcontrols2.conf

HEADERS += \
    enum_types.h \
    i2c_device.h \
    pwm_channels_settings.h \
    tablemodel.h

INCLUDEPATH += src/cpp /home/sergey/libs/

static {
    QTPLUGIN += qtvirtualkeyboardplugin
    QT += quickcontrols2 qml
    QT += svg
    QT += serialport
    QT += widgets
}
