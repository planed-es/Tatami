import QtQuick 2.12
import QtQuick.Controls 2.12
import com.planed.tatami 1.0 as Tatami

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
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.capitalization: Font.AllUppercase
    font.pixelSize: metrics.fontPixelSize
  }

  Tatami.AdaptiveFontSize {
    id: metrics
    text: root.text
    fontName: root.font.family
    width: root.width - root.leftPadding - root.rightPadding - 25
    maxSize: 20
    minSize: 8
  }

  Connections {
    target: root.model
    function onAttributeChanged() {
      root.text = displayFunction();
    }
  }
}
