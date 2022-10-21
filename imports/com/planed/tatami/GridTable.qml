import QtQuick 2.12
import QtQuick.Layouts 1.12

GridLayout {
  property var model
  property var header: []
  property var cells: []
  id: root
  columns: cells.length

  function cellComponentAt(row, column) {
    console.log("cellComponent at", row, column, cells[column]);
    return cells[column];
  }

  Repeater {
    model: header.length
    delegate: ColumnLabel {
      text: header[index]
    }
  }

  Repeater {
    model: root.model.length * root.columns
    delegate: Loader {
      property int rowIndex:    index / root.columns
      property int columnIndex: index % root.columns
      property var model:       root.model[rowIndex]
      sourceComponent: root.cellComponentAt(rowIndex, columnIndex)
    }
  }
}
