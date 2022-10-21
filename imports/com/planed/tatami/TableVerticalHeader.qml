import QtQuick 2.12

Column {
  property var   table
  property alias model: repeater.model

  x: root.contentX
  z: 2
  spacing: table.rowSpacing
  visible: table.model.rowHeaderColumn.length > 0
  Repeater {
    id: repeater
    model: parent.visible && table.rows > 0 ? table.rows : 0
    Label {
      width:             table.model.maxWidthForProperty(table.model.rowHeaderColumn, font.family, font.pointSize) + table.cellMargin * 2
      height:            table.rowHeightProvider(modelData)
      text:              table.model.headerData(modelData, Qt.Vertical)
      color:             table.style.headerTextColor
      font.family:       table.style.fontHeaderName
      font.pointSize:    table.style.fontHeaderPointSize
      padding:           table.cellMargin
      verticalAlignment: Text.AlignVCenter
      background: Rectangle { color: table.style.headerBackgroundColor }
    }
  }
}
