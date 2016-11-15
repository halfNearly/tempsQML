import QtQuick 2.6
import QtQuick.Controls 2.2

RadioButton {
    id: control

    indicator: Item {
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        width: 20; height: 20;
        clip: true

        Image {
            source: "images/checkboxes@2x.png"
            cache: false
            x: control.checked ? -60 : -40
            y: 0
            sourceSize.height: 20
            sourceSize.width: 140
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: "#000000"
        leftPadding: control.indicator.width + control.spacing
    }
}
