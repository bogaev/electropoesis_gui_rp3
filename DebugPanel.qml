import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Item {
    id: debugPanel

    property alias debugOutput: debugOutput

    function createDebugString(debug_map) {
        return "I2C handle: " + debug_map["handle"] + "\n" +
                "init_strerror: " + debug_map["init_strerror"] + "\n" +
                "stream_descr_err: " + debug_map["stream_descr_err"] + "\n" +
                "crc: " + debug_map["crc"] + "\n" +
                "ioctl_return: " + debug_map["ioctl_return"] + "\n" +
                "ioctl_strerror: " + debug_map["ioctl_strerror"] + "\n" +
                "write_return: " + debug_map["write_return"] + "\n" +
                "write_strerror: " + debug_map["write_strerror"] + "\n" +
                "on_write_status: " + debug_map["on_write_status"] + "\n" +
                "on_write_error: " + debug_map["on_write_error"] + "\n" +
                "on_bytes_transferred: " + debug_map["on_bytes_transferred"] + "\n" +
                "on_read_status: " + debug_map["on_write_error"] + "\n" +
                "on_read_error: " + debug_map["on_bytes_transferred"] + "\n" +
                "on_bytes_transferred: " + debug_map["on_bytes_transferred"] + "\n" +
                "msg_cnt: " + debug_map["msg_cnt"];
    }

    TextArea {
        id: debugOutput
        anchors.fill: parent
        readOnly: true
        wrapMode: TextArea.Wrap
    }

    Button {
        text: "Clear"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        onClicked: debugOutput.clear()
    }

    Connections {
        target: i2c_device
        onLog: {
            debugOutput.clear()
            debugOutput.append( createDebugString(debug_map) )
            console.log( createDebugString(debug_map) )
        }
    }
}
