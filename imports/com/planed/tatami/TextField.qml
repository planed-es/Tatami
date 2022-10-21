import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

QQControls.TextField {
  property color backgroundColor: "white"

  font.family: "Share Tech Mono"
  font.pixelSize: 20
  font.capitalization: Font.AllUppercase
  verticalAlignment: TextInput.AlignVCenter
  onActiveFocusChanged: {
    if (activeFocus)
      selectAll();
  }
  background: Rectangle {
    color: parent.readOnly ? "lightgray" : backgroundColor
    border.color: parent.activeFocus ? "blue" : "gray"
    border.width: 2
  }
  onAccepted: (KeyNavigation.tab || nextItemInFocusChain()).forceActiveFocus()
}
