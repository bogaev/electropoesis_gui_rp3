import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Imagine 2.3
import QtQuick.Controls.Universal 2.0
import QtQuick.Extras 1.4

import QtQml 2.12

Item {
    property bool allButtonState : false
    function toggleAllButtonState() {
        if(allButtonState) allButtonState = false;
        else allButtonState = true;
    }

    property variant checkBoxes:
        [checkBox0, checkBox1, checkBox2, checkBox3,
         checkBox4, checkBox5, checkBox6, checkBox7]

    Switch {
        id: switchSingleChoose
        x: 31
        y: 36
        width: 457
        height: 44
        text: qsTr("Одиночный выбор")
        font.pointSize: 24
        checked: true
        onClicked: {
            pwm_settings.qSingleSelect = checked
        }
    }

    Button {
        id: buttonToggleOnAllChannels
        x: 31
        y: 479
        width: 371
        height: 66
        text: qsTr("Переключить все")
        font.pointSize: 24
        onClicked: {
            toggleAllButtonState()
            if(allButtonState == true)
                pwm_settings.setAllChannels(Qt.Checked)
            else
                pwm_settings.setAllChannels(Qt.Unchecked)
        }
    }

    Column {
            x: -29
            y: 126
            width: 246
            height: 245
            spacing: 6

        CheckBox {
            id: checkBox0
            x: 58
            y: 196
            text: qsTr("Канал 1")
            font.pointSize: 24
            checked: true
            onClicked: {
                pwm_settings.setCheckboxStatus(0, checkState)
            }
        }

        CheckBox {
            id: checkBox1
            x: 58
            y: 242
            text: qsTr("Канал 2")
            spacing: 6
            font.pointSize: 24
            onClicked: {
                pwm_settings.setCheckboxStatus(1, checkState)
            }
        }

        CheckBox {
            id: checkBox2
            x: 58
            y: 288
            text: qsTr("Канал 3")
            font.pointSize: 24
            onClicked: {
                pwm_settings.setCheckboxStatus(2, checkState)
            }
        }

        CheckBox {
            id: checkBox3
            x: 58
            y: 334
            text: qsTr("Канал 4")
            font.pointSize: 24
            onClicked: {
                pwm_settings.setCheckboxStatus(3, checkState)
            }
        }
    }

    Column {
        x: 49
        y: 126
        width: 358
        height: 196
        spacing: 6
        CheckBox {
            id: checkBox4
            x: 180
            y: 196
            text: qsTr("Канал 5")
            font.pointSize: 24
            onClicked: {
                pwm_settings.setCheckboxStatus(4, checkState)
            }
        }

        CheckBox {
            id: checkBox5
            x: 180
            y: 242
            text: qsTr("Канал 6")
            font.pointSize: 24
            onClicked: {
                pwm_settings.setCheckboxStatus(5, checkState)
            }
        }

        CheckBox {
            id: checkBox6
            x: 180
            y: 288
            text: qsTr("Канал 7")
            font.pointSize: 24
            onClicked: {
                pwm_settings.setCheckboxStatus(6, checkState)
            }
        }

        CheckBox {
            id: checkBox7
            x: 180
            y: 334
            text: qsTr("Канал 8")
            font.pointSize: 24
            onClicked: {
                pwm_settings.setCheckboxStatus(7, checkState)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.66;height:1080;width:1980}
}
##^##*/
