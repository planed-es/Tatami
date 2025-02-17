import QtQuick 2.12
import "./TableHelpers.js" as Helpers
import "." as Tatami

TableView {
  id: root
  property int      cellMargin: 5
  property string   mainColumn: "name"
  property bool     navigationEnabled:     true
  property var      actionComponent:       null
  property var      style:                 applicationTableStyle
  property var      cellComponentProvider: function(row, column, propertyName) { return null; }
  property var      defaultCellDisplayer:  Helpers.defaultCellDisplayer;
  property var      rightAlignedCells:     ["amount","kgs","costByKg","expense"];
  property QtObject paginationComponent:   null
  readonly property int      currentIndex: model.currentIndex
  readonly property QtObject currentObject: model.modelAt(model.currentIndex)

  signal activated(int row, QtObject model)
  signal reloadRow(int row)

  function isOnLastRow() { return model.currentIndex + 1 >= rows; }
  function isOnFirstRow() { return model.currentIndex - 1 < 0; }

  function incrementPage() {
    if (paginationComponent.page <= paginationComponent.maxPage - 1) {
      model.currentIndex = 0;
      paginationComponent.nextPage();
    }
  }

  function decrementPage() {
    if (paginationComponent.page > 0) {
      paginationComponent.previousPage();
      model.currentIndex = rows - 1;
    }
  }

  function incrementIndex() {
    if (!isOnLastRow())
      model.currentIndex++;
    else if (paginationComponent)
      incrementPage();
    else
      model.currentIndex = 0;
  }

  function decrementIndex() {
    if (!isOnFirstRow())
      model.currentIndex--;
    else if (paginationComponent)
      decrementPage();
    else
      model.currentIndex = rows - 1;
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

  Tatami.TableStyle {
    id: applicationTableStyle
  }

  Tatami.TableStyle {
    id: inactiveTableStyle
    selectionColor: headerBackgroundColor
    backgroundColor: "#a1a1a1"
    altBackgroundColor: "#e2e2e2"
    headerTextColor: "#474747"
    selectionTextColor: textColor
  }

  Component {
    id: defaultCell
    Text {
      padding: root.cellMargin
      text: root.defaultCellDisplayer(value, propertyName)
      color: fontColor
      font.family: root.style.fontName
      font.pointSize: root.style.fontPointSize
      font.capitalization: Font.AllUppercase
      fontSizeMode: Text.HorizontalFit
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: root.rightAlignedCells.indexOf(propertyName) >= 0 ? Text.AlignRight : Text.AlignLeft
    }
  }

  Tatami.TableHorizontalHeader {
    id: columnsHeader
    table: root
  }

  Tatami.TableVerticalHeader {
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
