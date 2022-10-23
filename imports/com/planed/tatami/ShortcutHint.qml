import QtQuick 2.12
import QtQuick.Controls 2.12
import com.planed.tatami 1.0 as Tatami

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
    source: Tatami.Icons.url("accessories-character-map", 48)
  }
}
