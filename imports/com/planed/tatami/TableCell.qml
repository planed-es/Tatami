import QtQuick 2.12

Rectangle {
  id: root
  required property variant  displayValue
  required property QtObject model
  required property string   propertyName
  property var table
  property var defaultComponent
  property bool isPair: row % 2 > 0
  property color inactiveColor: isPair ? table.style.backgroundColor : table.style.altBackgroundColor
  property color fontColor: table.style.textColor

  function reload() {
    const customComponent = table.cellComponentProvider(row, cellComponent.index, cellComponent.propertyName);

    cellComponent.sourceComponent = customComponent || defaultComponent;
    if (column === 0 && table.activeRow === row)
      cellComponent.item.forceActiveFocus();
  }

  Component.onCompleted: reload();
  border.width: 1
  border.color: table.style.headerBackgroundColor
  color: row !== table.currentIndex ? inactiveColor : table.style.selectionColor

  states: [
    State {
      name: "active";   when: row === table.currentIndex && table.state == "active"
      PropertyChanges { target: root; color: table.style.selectionColor }
      PropertyChanges { target: root; fontColor: table.style.selectionTextColor }
    },
    State {
      name: "inactive"; when: row !== table.currentIndex || table.state != "active"
      PropertyChanges { target: root; color: inactiveColor }
      PropertyChanges { target: root; fontColor: table.style.textColor }
    }
  ]

  Loader {
    id: cellComponent
    property color    fontColor:    root.fontColor
    property string   propertyName: root.propertyName
    property QtObject model:        root.model
    property variant  value:        root.displayValue
    property int      index:        row
    property int      colIndex:     column
    anchors.fill: parent

    Connections {
      target: cellComponent.model
      function onAttributeChanged() {
        cellComponent.value = table.model.tableData(cellComponent.index, cellComponent.colIndex);
      }
    }
  }

  MouseArea {
    anchors.fill:    parent
    enabled:         table.navigationEnabled
    onClicked:       table.model.currentIndex = row
    onDoubleClicked: table.activated(row, cellComponent.model)
  }

  Connections {
    target: table
    function onCellComponentProviderChanged() { reload(); }
    function onReloadRow(rowIndex) { if (row === rowIndex) { reload(); } }
  }

  transitions: [
    Transition {
      to: "active";
      ColorAnimation { target: root; property: "color"; to: table.style.selectionColor; duration: 150 }
      ColorAnimation { target: root; property: "fontColor"; to: table.style.selectionTextColor; duration: 150 }
    },
    Transition {
      to: "inactive"
      ColorAnimation { target: root; property: "color"; to: inactiveColor; duration: 150 }
      ColorAnimation { target: root; property: "fontColor"; to: table.style.textColor; duration: 150 }
    }
  ]
}
