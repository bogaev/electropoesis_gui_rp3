import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import UsbTools 1.0

Item {
    id: mainPanel
    anchors.fill: parent
    property int dev_cnt: 0
    property int upd_cnt: 0

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            TabButton {
                text: "Debug Output"
            }
            TabButton {
                text: "Devices List"
            }
        }

        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            Item {
                Flickable {
                    anchors.fill: parent
                    contentWidth: textArea.contentWidth
                    flickableDirection: Flickable.HorizontalFlick

                    TextArea {
                        id: textArea
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: contentWidth > parent.width ? contentWidth : parent.width
                        readOnly: true
                        wrapMode: TextArea.NoWrap
                    }
                }

                Button {
                    text: "Exit"
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    onClicked: Qt.callLater(Qt.quit)
                }

                Button {
                    text: "Clear"
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    onClicked: textArea.clear()
                }

                Connections {
                    target: i2c_device
                    onLog: {
                        textArea.clear()
                        textArea.append(createDebugString(debug_map))
                        console.log(createDebugString(debug_map))
                    }
                }
            }

            Item {
                ListView {
                    id: devicesListView
                    width: parent.width
                    clip: true
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    model: ListModel {
                        id: devicesListModel
                    }

                    header: Text {
                        color: "white"
                        text: "USB updates count: " + mainPanel.upd_cnt + "\n" +
                              "USB devices count: " + mainPanel.dev_cnt + "\n\n"
                    }

                    delegate: Item {
                        width: devicesListView.width
                        height: delegateFlickable.height + 5

                        Flickable {
                            id: delegateFlickable
                            width: parent.width
                            contentWidth: textItem.width + 10
                            height: textItem.height
                            flickableDirection: Flickable.HorizontalFlick
                            boundsBehavior: Flickable.StopAtBounds

                            Text {
                                id: textItem
                                color: "white"
                                text: "Device " + (index + 1) + ": " + modelData
                                wrapMode: Text.NoWrap
                                width: implicitWidth
                            }
                        }
                    }
                }

                UsbMonitor {
                    id: usbMonitor

                    onUsbChanged: {
                        let devices = devicesList;
                        upd_cnt += 1
                        dev_cnt = devices.length
                        devicesListModel.clear();
                        for (let i = 0; i < devices.length; i++) {
                            devicesListModel.append({"modelData": devices[i]});
                        }
                    }
                }
            }
        }
    }


    function comStatusToString(status) {
        switch(status) {
            case 0: return "COM_STATUS_OK";
            case 1: return "COM_STATUS_CRC_ERR";
            case 2: return "COM_STATUS_OS_ERR";
            case 3: return "COM_STATUS_ERR";
            default: return "Unknown COM_STATUS";
        }
    }

    function mcuStatusToString(status) {
        switch(status) {
            case 0: return "MCU_STATUS_RESTARTED";
            case 1: return "MCU_STATUS_RELAY_ON";
            case 2: return "MCU_STATUS_RESUME";
            case 3: return "MCU_STATUS_PAUSE";
            case 4: return "MCU_STATUS_ERR";
            default: return "Unknown MCU_STATUS";
        }
    }

    function createDebugString(debug_map) {
        return "I2C handle: " + debug_map["handle"] + "\n" +
                "I2C init error: " + debug_map["init_strerror"] + "\n" +

                "Message CRC: " + debug_map["crc"] + "\n" +
                "ioctl return: " + debug_map["ioctl_return"] + "\n" +
                "ioctl error: " + debug_map["ioctl_strerror"] + "\n" +

                "Set param com stat: " + comStatusToString(debug_map["Com status"]) + "\n" +
                "Set param MCU stat: " + mcuStatusToString(debug_map["MCU status"]) + "\n" +

                "Main MCU comm: " + comStatusToString(debug_map["Main MCU com answer"]) + "\n" +
                "Main MCU status: " + mcuStatusToString(debug_map["Main MCU answer"]) + "\n" +

                "Add MCU comm: " + comStatusToString(debug_map["Add MCU com answer"]) + "\n" +
                "Add MCU status: " + mcuStatusToString(debug_map["Add MCU answer"]) + "\n" +

                "Total msg count: " + debug_map["msg_cnt"] + "\n" +
                "Total errors count: " + debug_map["err_cnt"];
    }
}
