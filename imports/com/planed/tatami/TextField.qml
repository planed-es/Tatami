import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls
import com.planed.tatami 1.0 as Tatami

QQControls.TextField {
  id: root
  property color backgroundColor: "white"

  font.family: "Share Tech Mono"
  font.pixelSize: metrics.fontPixelSize
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

  Tatami.AdaptiveFontSize {
    id: metrics
    text: root.text
    fontName: root.font.family
    width: root.width - root.leftPadding - root.rightPadding
    maxSize: 20
    minSize: 12
  }
}
