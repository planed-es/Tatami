import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.labs.settings 1.1

ApplicationWindow {
  id: root
  property bool isFullScreen: false

  width: 1024
  height: 800
  visible: true
  onIsFullScreenChanged: updateDisplay()
  Component.onCompleted: updateDisplay()

  function updateDisplay() {
    if (isFullScreen)
      root.showFullScreen();
    else
      root.showNormal();
  }

  Settings {
    property alias x: root.x
    property alias y: root.y
    property alias width: root.width
    property alias height: root.height
    property alias fullScreen: root.isFullScreen
  }

  Shortcut {
    sequence: "F11"
    onActivated: isFullScreen = !isFullScreen;
  }
}
