import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
  id: root
  property QtObject model
  property var displayFunction

  signal accepted();

  onDisplayFunctionChanged: text = displayFunction()
  Keys.onEnterPressed: accepted()
  Keys.onReturnPressed: accepted()
  onAccepted: (KeyNavigation.tab || nextItemInFocusChain()).forceActiveFocus()
  background: Rectangle {
    color: parent.activeFocus ? "#3E65FF" : "#CCCCCC"
  }
  Connections {
    target: root.model
    function onAttributeChanged() {
      root.text = displayFunction();
    }
  }
}
