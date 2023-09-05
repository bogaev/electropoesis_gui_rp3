import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Imagine 2.3
import QtQuick.Controls.Universal 2.0
import QtQuick.Extras 1.4

import QtQuick.VirtualKeyboard 2.1
import QtQml 2.12

import PwmSettings 0.1

ApplicationWindow {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr(" ")
    property int height_shift: 200

    Material.theme: Material.Dark
    Material.accent: Material.Teal

    GroupBox {
        id: debugBox
        title: "Debug Panel"
        x: 1050
        y: 650
        width: 400
        height: 400
        z: 2

        DebugPanel {
            id: debugPanel
            anchors.fill: parent
        }
    }

    Text {
        id: text1
        x: 0
        y: 0
        width: 193
        height: 40
        color: "#ffffff"
        text: qsTr("v 0.010")
        font.pixelSize: 10
    }

    PwmSettings {
        id: pwm_settings
        onCheckboxChanged: {
            ccb.checkBoxes[index].checkState = status
        }
        onSingleCheckboxChanged: {
            sspCarrier.updateSignalsParams(index)
            sspAmpMod.updateSignalsParams(index)
            sspFreqMod.updateSignalsParams(index)
        }
        onUartStatusChanged: {
            debugTextArea.text = newStatus
        }
        onIsNonCosCarrier: {
            if (value == true) {
                sspAmpMod.visible = false
                sspFreqMod.visible = false
            } else {
                sspAmpMod.visible = true
                sspFreqMod.visible = true
            }
        }
    }

    Connections {
        id: i2c
        target: i2c_device
    }

    Component.onCompleted: {
        sspCarrier.setSignalName("Несущая")
        sspAmpMod.setSignalName("Ампл. мод.")
        sspFreqMod.setSignalName("Част. мод.")
        sspCarrier.setSignalType(1)
    }

    ListModel {
        id: carrierSignalType
        ListElement { text: "Отсутствует" }
        ListElement { text: "Синусоидальный" }
        ListElement { text: "Меандр" }
        ListElement { text: "Треугольный" }
        ListElement { text: "Пилообразный" }
    }

    ListModel {
        id: modSignalType
        ListElement { text: "Отсутствует" }
        ListElement { text: "Синусоидальный" }
        ListElement { text: "Меандр" }
        ListElement { text: "Треугольный" }
        ListElement { text: "Пилообразный" }
    }

    ListModel {
        id: emptySignalType
        ListElement { text: "Отсутствует" }
    }

    ChannelsCheckboxes {
        id: ccb
        width: parent.width - 2
    }

    SetSignalParams {
        id: sspCarrier
        signalId: 0
        signalsList: carrierSignalType
        y: parent.y
        width: parent.width - 2
    }

    SetSignalParams {
        id: sspAmpMod
        signalId: 1
        signalsList: modSignalType
        y: parent.y + height_shift
        width: parent.width - 2
    }

    SetSignalParams {
        id: sspFreqMod
        signalId: 2
        signalsList: modSignalType
        y: parent.y + height_shift*2
        width: parent.width - 2
    }

    SettingsTable {
        id: settingsTable
        anchors.fill: parent
        anchors.bottomMargin: 6
        anchors.leftMargin: 30
        anchors.rightMargin: 872
        anchors.topMargin: 624
    }

    Button {
        id: buttonSignalSet
        x: 1507
        y: 703
        width: 332
        height: 91
        text: qsTr("Установить")
        font.pointSize: 24
        onClicked: {
            i2c_device.send(1, 0xDEADBEEF);
//            sspCarrier.setSignalParams(sspCarrier.signalId)
//            sspCarrier.fillSettingsTable()
//            sspAmpMod.setSignalParams(sspAmpMod.signalId)
//            sspAmpMod.fillSettingsTable()
//            sspFreqMod.setSignalParams(sspFreqMod.signalId)
//            sspFreqMod.fillSettingsTable()
//            pwm_settings.commitChanges()
        }
    }

    InputPanel {
        id: inputPanel
        z: 89
        y: active ? parent.height - height+50 : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Button {
        id: button
        x: 1507
        y: 918
        width: 332
        height: 91
        text: qsTr("Выход")
        font.pointSize: 24
        onClicked: {
            Qt.callLater(Qt.quit)
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}
}
##^##*/
