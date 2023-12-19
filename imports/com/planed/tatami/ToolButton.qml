import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

QQControls.ToolButton {
  id: control
  property string label: {
    var src = action.text;

    src  = action.text.charAt(0).toUpperCase();
    src += action.text.slice(1);
    return src;
  }
  //icon.color: null
  font.pixelSize: 18
  icon.width: 32
  icon.height: 32

  text: label + (action.shortcut ? "<br/>" : "")

  Text {
    text: action.shortcut ? `<i>${action.shortcut.nativeText}</i>` : ""
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 3
    anchors.left: parent.left
    anchors.leftMargin: 45
    font.pixelSize: 16
    color: control.enabled ? "black" : "grey"
  }
}
