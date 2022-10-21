import QtQuick 2.12

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
    Label {
      width:               table.columnWidthProvider(modelData)
      height:              35
      text:                table.model.headerData(modelData, Qt.Horizontal)
      color:               table.style.headerTextColor
      font.family:         table.style.fontHeaderName
      font.pointSize:      table.style.fontHeaderPointSize
      font.capitalization: Font.Capitalize
      font.weight:         Font.Bold
      padding:             table.cellMargin
      verticalAlignment:   Text.AlignVCenter
      clip:                true
      background: Rectangle { color: table.style.headerBackgroundColor }
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
