import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Imagine 2.3
import QtQuick.Controls.Universal 2.0
import QtQuick.Extras 1.4

import QtQml 2.12

Item {
    width: 1980
    height: 1080

    property int signalId: 0
    property ListModel signalsList: carrierSignalType
    property bool paramChanged: false
    property bool signalTypeChanged: false

    property variant arSignal: [0, 0, 0, 0, 0, 0, 0, 0]
    property variant arAmp: [0, 0, 0, 0, 0, 0, 0, 0]
    property variant arFreq: [0, 0, 0, 0, 0, 0, 0, 0]

    function is_params_changed() {
        var ret = (get_signal_type(comboBoxSignalType.currentText) !== 4) && paramChanged
        ret = ret || signalTypeChanged
        paramChanged = false
        signalTypeChanged = false
        return ret;
    }

    function setSignalParams(signal) {
        pwm_settings.setSignalParams(
                    signal,
                    comboBoxSignalType.currentIndex,
                    comboBoxAmplitude.currentIndex+1,
                    comboBoxFreq.currentIndex+1)
    }

    function setSignalName(name) {
        labelSignalName.text = name
    }

    function setSignalType(val) {
        comboBoxSignalType.currentIndex = val
    }

    function saveIndexes() {
        for(var i = 0; i < 8; ++i) {
            if(ccb.checkBoxes[i].checkState === Qt.Checked) {
                arSignal[i] = comboBoxSignalType.currentIndex
                arAmp[i] = comboBoxAmplitude.currentIndex
                arFreq[i] = comboBoxFreq.currentIndex
            }
        }
    }

    function concatenateParamsString() {
        var type = ["-", "Син", "Меандр", "Треуг", "Пила"]
        if(comboBoxSignalType.currentIndex == 0) return "-"
        return type[comboBoxSignalType.currentIndex] + ", " + comboBoxAmplitude.currentText + "В, " + comboBoxFreq.currentText + "Гц"
    }

    function fillSettingsTable() {
        saveIndexes()
        for(var i = 0; i < 8; ++i) {
            if(ccb.checkBoxes[i].checkState === Qt.Checked)
                settingsTable.model.setCellText(i, signalId, concatenateParamsString())
        }
    }

    function updateSignalsParams(index) {
        comboBoxSignalType.currentIndex = arSignal[index]
        comboBoxAmplitude.currentIndex = arAmp[index]
        comboBoxFreq.currentIndex = arFreq[index]
    }

    ListModel {
        id: listAmplitudes
        ListElement { text: "1" }
        ListElement { text: "2" }
        ListElement { text: "3" }
    }

    ListModel {
        id: listFreq
        ListElement { text: "1" }
        ListElement { text: "5" }
        ListElement { text: "10" }
        ListElement { text: "20" }
        ListElement { text: "30" }
        ListElement { text: "40" }
        ListElement { text: "50" }
        ListElement { text: "100" }
        ListElement { text: "150" }
        ListElement { text: "200" }
        ListElement { text: "250" }
        ListElement { text: "300" }
        ListElement { text: "400" }
        ListElement { text: "450" }
        ListElement { text: "500" }
        ListElement { text: "600" }
        ListElement { text: "700" }
        ListElement { text: "800" }
        ListElement { text: "900" }
        ListElement { text: "1000" }
    }

    Label {
        id: labelSignalName
        x: 518
        y: 120
        width: 185
        height: 37
        text: qsTr("Несущая")
        font.pointSize: 24
        fontSizeMode: Text.Fit
    }

    Label {
        id: labelSignalType
        x: 709
        y: 53
        width: 204
        height: 37
        text: qsTr("Тип сигнала")
        font.pointSize: 24
        fontSizeMode: Text.Fit
    }

    ComboBox {
        id: comboBoxSignalType
        x: 709
        y: 105
        width: 350
        height: 67
        font.pointSize: 24
        model: signalsList
        onCurrentIndexChanged: {
        }
    }

    Button {
        id: sig_type_up
        x: 1071
        y: 74
        width: 83
        height: 67
        text: "\u25b2"
        font.pointSize: 24
        display: AbstractButton.TextOnly
        onClicked: {
            comboBoxSignalType.incrementCurrentIndex()
            setSignalParams(signalId)
            fillSettingsTable()
            pwm_settings.commitChanges()
        }
    }

    Button {
        id: sig_type_dn
        x: 1071
        y: 141
        width: 83
        height: 67
        text: "\u25bc"
        display: AbstractButton.TextOnly
        font.pointSize: 24
        onClicked: {
            comboBoxSignalType.decrementCurrentIndex()
            setSignalParams(signalId)
            fillSettingsTable()
            pwm_settings.commitChanges()
        }
    }

    Label {
        id: labelAmplitude
        x: 1174
        y: 48
        width: 248
        height: 37
        text: qsTr("Размах сигнала, В")
        font.pointSize: 24
        fontSizeMode: Text.Fit
    }

    ComboBox {
        id: comboBoxAmplitude
        x: 1174
        y: 105
        width: 248
        height: 67
        font.pointSize: 24
        model: listAmplitudes
        onCurrentIndexChanged: {
        }
    }

    Button {
        id: amp_up
        x: 1433
        y: 74
        width: 83
        height: 67
        text: "\u25b2"
        display: AbstractButton.TextOnly
        font.pointSize: 24
        onClicked: {
            comboBoxAmplitude.incrementCurrentIndex()
            setSignalParams(signalId)
            fillSettingsTable()
            pwm_settings.commitChanges()
        }
    }

    Button {
        id: amp_dn
        x: 1433
        y: 141
        width: 83
        height: 67
        text: "\u25bc"
        display: AbstractButton.TextOnly
        font.pointSize: 24
        onClicked: {
            comboBoxAmplitude.decrementCurrentIndex()
            setSignalParams(signalId)
            fillSettingsTable()
            pwm_settings.commitChanges()
        }
    }

    Label {
        id: labelFreq
        x: 1538
        y: 48
        width: 265
        height: 42
        text: qsTr("Частота сигнала, Гц")
        font.pointSize: 24
        fontSizeMode: Text.Fit
    }

    ComboBox {
        id: comboBoxFreq
        x: 1538
        y: 105
        width: 248
        height: 67
        font.pointSize: 24
        editable: true
        inputMethodHints: Qt.ImhDigitsOnly
        model: listFreq
        onCurrentIndexChanged: {
        }
    }

    Button {
        id: freq_up
        x: 1799
        y: 74
        width: 83
        height: 67
        text: "\u25b2"
        display: AbstractButton.TextOnly
        font.pointSize: 24
        onClicked: {
            comboBoxFreq.incrementCurrentIndex()
            setSignalParams(signalId)
            fillSettingsTable()
            pwm_settings.commitChanges()
        }
    }

    Button {
        id: freq_dn
        x: 1799
        y: 141
        width: 83
        height: 67
        text: "\u25bc"
        display: AbstractButton.TextOnly
        font.pointSize: 24
        onClicked: {
            comboBoxFreq.decrementCurrentIndex()
            setSignalParams(signalId)
            fillSettingsTable()
            pwm_settings.commitChanges()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}D{i:34}
}
##^##*/
