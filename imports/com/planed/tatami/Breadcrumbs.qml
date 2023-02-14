import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
  id: breadcrumbsRow
  property var stackView
  property var colorPalette
  height: 40
  color: "transparent"

  signal depthClicked(int depth)

  function formatName(viewName)  {
    return viewName.charAt(0).toUpperCase() + viewName.slice(1);
  }

  Rectangle {
    color: colorPalette ? colorPalette.dark : "black"
    anchors { bottom: parent.bottom; left: parent.left; right: parent.right }
    height: 1
  }

  Row {
    anchors.fill: parent
    anchors.margins: 5
    spacing: 5
    Repeater {
      id: breadcrumbsRepeater
      model: stackView.depth
      delegate: Row {
        property var view: stackView.get(index)
        spacing: 5
        Button {
          text:      parent.view ? formatName(parent.view.viewName) : ""
          icon.name: parent.view ? parent.view.viewIcon : ""
          icon.color: "transparent"
          font.family: "Signika"
          font.pointSize: 15
          height: breadcrumbsRow.height - 10
          onClicked: depthClicked(index)
        }
        Label {
          height: breadcrumbsRow.height - 10
          verticalAlignment: Qt.AlignVCenter
          text: '»'
          visible: index < stackView.depth - 1
        }
      }
    }
  }
}
