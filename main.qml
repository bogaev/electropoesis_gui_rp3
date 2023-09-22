import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Imagine 2.3
import QtQuick.Controls.Universal 2.0
import QtQuick.Extras 1.4

import QtQuick.VirtualKeyboard 2.1
import QtQml 2.12

ApplicationWindow {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr(" ")
    property int height_shift: 200

    Material.theme: Material.Dark
    Material.accent: Material.Teal

    readonly property int comStatus_OK: 0
    readonly property int comStatus_CRC_ERR: 1
    readonly property int comStatus_OS_ERR: 2
    readonly property int comStatus_ERR: 3

    readonly property int mcuStatus_RESTARTED: 0
    readonly property int mcuStatus_RELAY_ON: 1
    readonly property int mcuStatus_RESUME: 2
    readonly property int mcuStatus_PAUSE: 3
    readonly property int mcuStatus_ERR: 4

    property int mcuMain: 0xAA
    property int mcuAdd: 0x55

    property int mcuMainStatus: 0
    property int mcuAddStatus: 0

    property bool isRunning: false

    function applySignalParams() {
        sspCarrier.setSignalParams(sspCarrier.signalId)
        sspCarrier.fillSettingsTable()
        sspAmpMod.setSignalParams(sspAmpMod.signalId)
        sspAmpMod.fillSettingsTable()
        sspFreqMod.setSignalParams(sspFreqMod.signalId)
        sspFreqMod.fillSettingsTable()
        pwm_settings.commitChanges()
    }

    Connections {
        target: pwm_settings
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
        onIsNonCosCarrier: function(value) {
            if (value === true) {
                sspAmpMod.visible = false
                sspFreqMod.visible = false
            } else {
                sspAmpMod.visible = true
                sspFreqMod.visible = true
            }
        }
        onSignalParamChanged: function(ch, chbx, sig, type, amp, freq) {
            console.log("Signal received:", ch, chbx, sig, type, amp, freq);
            ccb.checkBoxes[ch].checkState = chbx ? Qt.Checked : Qt.Unchecked
            if (sig === 0) {
                sspCarrier.setParams(ch, type, amp, freq)
                sspCarrier.updateSignalsParams(ch)
            } else if (sig === 1) {
                sspAmpMod.setParams(ch, type, amp, freq)
                sspAmpMod.updateSignalsParams(ch)
            } else if (sig === 2) {
                sspFreqMod.setParams(ch, type, amp, freq)
                sspFreqMod.updateSignalsParams(ch)
            }
        }
    }

    Component.onCompleted: {
        sspCarrier.setSignalName("Несущая")
        sspAmpMod.setSignalName("Ампл. мод.")
        sspFreqMod.setSignalName("Част. мод.")
        pwm_settings.initAllChannels()
        applySignalParams()
    }

    Timer {
        id: pwmTimer
        interval: 1000
        running: true   // Автоматический запуск таймера
        repeat: true    // Повторять бесконечно

        onTriggered: {
            mcuMainStatus = pwm_settings.getStatus(mcuMain)
            mcuAddStatus = pwm_settings.getStatus(mcuAdd)
            isRunning = mcuMainStatus == mcuStatus_RESUME
                     && mcuAddStatus == mcuStatus_RESUME
            if (    mcuMainStatus == mcuStatus_RESTARTED
                 || mcuAddStatus == mcuStatus_RESTARTED) {
                pwm_settings.relayOn()
                pwm_settings.initAllChannels()
                applySignalParams()
            }
//            if (    sspCarrier.isParamsChanged()
//                 || sspAmpMod.isParamsChanged()
//                 || sspFreqMod.isParamsChanged()
////                 || ccb.isParamsChanged()
//                    ) {
//                applySignalParams()
//            }
        }
    }

    GroupBox {
        id: debugBox
        title: "Debug Panel"
        x: 1050
        y: 650
        width: 400
        height: 400
        z: 2
        visible: false

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
        text: qsTr("v 0.011")
        font.pixelSize: 10

        MouseArea {
            anchors.fill: parent
            onClicked: {
                debugBox.visible = !debugBox.visible // изменение свойства visible на противоположное значение
            }
        }
    }

    Connections {
        target: i2c_device
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
        id: buttonSet
        x: 1507
        y: 803
        width: 332
        height: 91
        text: qsTr("Установить")
        font.pointSize: 24
        anchors.right: buttonStart.right
        anchors.bottom: buttonStart.top
        anchors.bottomMargin: 40

        onClicked: {
            applySignalParams()
        }
    }

    Button {
        id: buttonStart
        x: 1507
        y: 803
        width: 332
        height: 91
        text: qsTr("Старт / Стоп")
        font.pointSize: 24
        anchors.right: parent.right
        anchors.rightMargin: 40
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100

        onClicked: {
            if (isRunning) {
                pwm_settings.stop()
            } else {
                pwm_settings.start()
            }
        }
    }

    Rectangle {
        id: animatedIndicatorStart
        x: 1480
        y: 803
        width: 20
        height: 20
        radius: width / 2
        anchors.left: buttonStart.left
        anchors.leftMargin: 10
        anchors.top: buttonStart.top
        anchors.topMargin: 15

        property bool isGreen: false

        color: {
            if (!isRunning) {
                return "red";
            } else {
                if (isGreen) {
                    return "green";
                } else {
                    return "black";
                }
            }
        }

        Timer {
            interval: 1000
            running: isRunning
            repeat: true
            onTriggered: {
                animatedIndicatorStart.isGreen = !animatedIndicatorStart.isGreen;
            }
        }
    }

    InputPanel {
        id: inputPanel
        z: 89
        y: active ? parent.height - height+50 : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}
}
##^##*/
