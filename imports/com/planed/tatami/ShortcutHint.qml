import QtQuick 2.12
import QtQuick.Controls 2.12

Label {
  visible: text.length > 0
  leftPadding: 30
  rightPadding: 5
  bottomPadding: 5

  Image {
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.leftMargin: 5
    width: 20
    height: 20
    source: "qrc:/icons/icon-theme/48x48/accessories-character-map.png"
  }
}
