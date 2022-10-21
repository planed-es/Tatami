import QtQuick 2.12
import "./TableHelpers.js" as Helpers

TableView {
  id: root
  property int    currentIndex: model.currentIndex
  property int    cellMargin: 5
  property string mainColumn: "name"
  property bool   navigationEnabled:     true
  property var    actionComponent:       null
  property var    style:                 applicationTableStyle
  property var    cellComponentProvider: function(row, column, propertyName) { return null; }
  property var    defaultCellDisplayer:  Helpers.defaultCellDisplayer;
  property var    rightAlignedCells: ["amount","kgs","costByKg","expense"];
  property QtObject currentObject: model.modelAt(model.currentIndex)

  signal activated(int row, QtObject model)
  signal reloadRow(int row)

  function incrementIndex() {
    if (model.currentIndex + 1 >= rows)
      model.currentIndex = 0;
    else
      model.currentIndex++;
  }

  function decrementIndex() {
    if (model.currentIndex - 1 < 0)
      model.currentIndex = rows - 1;
    else
      model.currentIndex--;
  }

  columnSpacing: 1
  rowSpacing:    1
  leftMargin:    rowsHeader.visible ? rowsHeader.width : 0
  topMargin:     columnsHeader.height
  clip:          true
  reuseItems:    false
  columnWidthProvider:   Helpers.columnWidthProvider.bind(root)
  rowHeightProvider:     Helpers.rowHeightProvider.bind(root)
  onCurrentIndexChanged: Helpers.ensureCurrentIndexVisibility.bind(root, columnsHeader)
  onWidthChanged:        Helpers.refreshLayout.bind(root, columnsHeader, rowsHeader)()
  onRowsChanged: {
    Helpers.refreshLayout.bind(root, columnsHeader, rowsHeader)()
  }

  states: [
    State {
      name: "active"
      when: navigationEnabled || actionComponent
      PropertyChanges {
        target: root
        style: applicationTableStyle
      }
    },
    State {
      name: "inactive"
      when: !navigationEnabled && !actionComponent
      PropertyChanges {
        target: root
        style: inactiveTableStyle
      }
    }
  ]

  delegate: TableCell {
    table: root
    defaultComponent: defaultCell
  }

  TableStyle {
    id: applicationTableStyle
  }

  TableStyle {
    id: inactiveTableStyle
    selectionColor: headerBackgroundColor
    backgroundColor: "#a1a1a1"
    altBackgroundColor: "#e2e2e2"
    headerTextColor: "#474747"
  }

  Component {
    id: defaultCell
    Text {
      padding: root.cellMargin
      text: root.defaultCellDisplayer(value, propertyName)
      font.family: root.style.fontName
      font.pointSize: root.style.fontPointSize
      font.capitalization: Font.AllUppercase
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: root.rightAlignedCells.indexOf(propertyName) >= 0 ? Text.AlignRight : Text.AlignLeft
    }
  }

  TableHorizontalHeader {
    id: columnsHeader
    table: root
  }

  TableVerticalHeader {
    id: rowsHeader
    table: root
  }

  Loader {
    sourceComponent: {
      if (actionComponent)
        return actionComponent;
      return navigationEnabled ? standardNavigation : null;
    }
    Component {
      id: standardNavigation
      TableNavigation {
        id: navigation
        table: root
      }
    }
  }
}
