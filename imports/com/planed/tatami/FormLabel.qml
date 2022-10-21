import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
  id: root
  property string label
  property bool   withQuickAccess
  property string quickAccessKey
  implicitHeight: labelColumn.height
  implicitWidth: labelColumn.width

  states: [
    State { name: "activeLabel"; when: isActive },
    State { name: "inactiveLabel"; when: !isActive }
  ]

  transitions: [
    Transition {
      to: "inactiveLabel"
      ColorAnimation { target: labelView; property: "color"; from: "#3E65FF"; to: "black"; duration: 200 }
      ColorAnimation { target: rect;      property: "color"; from: "#3E65FF"; to: "#CCCCCC"; duration: 200 }
    },
    Transition {
      to: "activeLabel"
      ColorAnimation { target: labelView; property: "color"; to: "#3E65FF"; from: "black"; duration: 200 }
      ColorAnimation { target: rect;      property: "color"; to: "#3E65FF"; from: "#CCCCCC"; duration: 200 }
    }
  ]

  Rectangle {
    id: rect
    height: parent.height
    width: 5
    color: "#CCCCCC"
  }

  Column {
    id: labelColumn
    Label {
      id: labelView
      text: root.label
      font.pixelSize: 20
      font.capitalization: Font.Capitalize
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
    }

    ShortcutHint {
      visible: withQuickAccess
      text: quickAccessKey
    }
  }
}
