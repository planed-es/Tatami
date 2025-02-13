import QtQuick 2.12
import QtQuick.Controls 2.12

Row {
  property var   table
  property alias model: repeater.model

  y: table.contentY
  z: 2
  spacing: table.columnSpacing

  Connections {
    target: table.model
    function onColumnsChanged() {
      repeater.model = table.model.columnCount;
    }
  }

  Repeater {
    id: repeater
    model: table.model.columnCount
    Rectangle {
      id:     container
      width:  table.columnWidthProvider(modelData)
      height: 35
      z:      mouseArea.containsMouse ? 9999 : index
      clip:   !mouseArea.containsMouse
      color:  table.style.headerBackgroundColor
      MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
      }
      Label {
        property int extraWidth: label.width - container.width
        id:                  label
        text:                table.model.headerData(modelData, Qt.Horizontal)
        color:               table.style.headerTextColor
        font.family:         table.style.fontHeaderName
        font.pointSize:      table.style.fontHeaderPointSize
        font.capitalization: Font.Capitalize
        font.weight:         Font.Bold
        padding:             table.cellMargin
        verticalAlignment:   Text.AlignVCenter
        background:          Rectangle {
          color: table.style.headerBackgroundColor
          border.color: "black"
          border.width: mouseArea.containsMouse ? 1 : 0
          visible: label.extraWidth > 0
        }
        SequentialAnimation {
          running: label.extraWidth > 0 && !mouseArea.containsMouse
          loops: Animation.Infinite
          NumberAnimation {
            target: label; property: "x"; to: -label.extraWidth; duration: Math.max(1, label.extraWidth * 200)
          }
          NumberAnimation {
            target: label; property: "x"; to: 0; duration: Math.max(1, label.extraWidth * 200)
          }
        }
      }
      Timer {
        interval: 1000
        running: true
        onTriggered: {
          width = table.columnWidthProvider(modelData);
        }
      }
    }
  }
}
