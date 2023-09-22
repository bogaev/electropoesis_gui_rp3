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

    property int channelsMax: 8
    property int signalId: 0
    property ListModel signalsList: carrierSignalType
    property bool paramChanged: false
    property bool signalTypeChanged: false

    property variant arSignal:  [0, 0, 0, 0, 0, 0, 0, 0]
    property variant arAmp:     [0, 0, 0, 0, 0, 0, 0, 0]
    property variant arFreq:    [0, 0, 0, 0, 0, 0, 0, 0]

    function isParamsChanged() {
        var ret = paramChanged
        paramChanged = false
        return ret;
    }

    function getAmpValue() {
        var ampValue = parseInt(comboBoxAmplitude.editText.trim());
        return ampValue;
    }

    function getFreqValue() {
        var freqValue = parseInt(comboBoxFreq.editText.trim());
        return freqValue;
    }

    function setSignalParams(signal) {
        pwm_settings.setSignalParams(
                    signal,
                    comboBoxSignalType.currentIndex,
                    getAmpValue(),
                    getFreqValue())
    }

    function setSignalName(name) {
        labelSignalName.text = name
    }

    function setSignalType(val) {
        comboBoxSignalType.currentIndex = val
    }

    function saveIndexes() {
        for(var i = 0; i < channelsMax; ++i) {
            if(ccb.checkBoxes[i].checkState === Qt.Checked) {
                arSignal[i] = comboBoxSignalType.currentIndex
                arAmp[i] = getAmpValue()
                arFreq[i] = getFreqValue()
            }
        }
    }

    function setParams(ch, type, amp, freq) {
        arSignal[ch] = type
        arAmp[ch] = amp
        arFreq[ch] = freq
    }

    function concatenateParamsString() {
        var type = ["-", "Син", "Меандр", "Треуг", "Пила"]
        if (comboBoxSignalType.currentIndex == 0) return "-"
        return type[comboBoxSignalType.currentIndex] + ", " + getAmpValue() + "В, " + getFreqValue() + "Гц"
    }

    function fillSettingsTable() {
        saveIndexes()
        for(var i = 0; i < channelsMax; ++i) {
            if(ccb.checkBoxes[i].checkState === Qt.Checked)
                settingsTable.model.setCellText(i, signalId, concatenateParamsString())
        }
    }

    function updateSignalsParams(index) {
        comboBoxSignalType.currentIndex = arSignal[index]
        comboBoxAmplitude.editText = arAmp[index]
        comboBoxFreq.editText = arFreq[index]
    }

    ListModel {
        id: listAmplitudes
        ListElement { text: "1" }
        ListElement { text: "2" }
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
            paramChanged = true
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
            paramChanged = true
//            setSignalParams(signalId)
//            fillSettingsTable()
//            pwm_settings.commitChanges()
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
            paramChanged = true
        }
    }

    Label {
        id: labelAmplitude
        x: 1174
        y: 48
        width: 248
        height: 37
        text: qsTr("Ампл. коэфф")
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
        editable: true
        model: listAmplitudes
        onCurrentIndexChanged: {
            paramChanged = true
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
            paramChanged = true
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
            paramChanged = true
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
            paramChanged = true
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
            paramChanged = true
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
            paramChanged = true
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}D{i:34}
}
##^##*/
