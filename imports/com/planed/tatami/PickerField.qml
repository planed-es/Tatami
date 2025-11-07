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
  contentItem: Label {
    text: root.text
    padding: 2
    clip: true
    fontSizeMode: Text.HorizontalFit
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.capitalization: Font.AllUppercase
  }
  Connections {
    target: root.model
    function onAttributeChanged() {
      root.text = displayFunction();
    }
  }
}
