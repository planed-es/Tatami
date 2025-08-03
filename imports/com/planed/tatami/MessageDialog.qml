import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls
import QtQuick.Layouts 1.12
import com.planed.tatami 1.0

Dialog {
  property string message
  property string icon: "security-medium"

  height: Math.min(flickable.contentHeight, parent.height * 0.25)

  Flickable {
    id: flickable
    anchors.fill: parent
    contentHeight: dialogView.height
    clip: true

    RowLayout {
      id: dialogView

      Image {
        Layout.alignment: (dialogView.height > 250 ? Qt.AlignTop : Qt.AlignVCenter) | Qt.AlignHCenter
        source: Icons.url(icon, 48)
      }

      QQControls.Label {
        id: label
        text: message
        Layout.fillWidth: true
      }
    }
  }

  Rectangle {
    id: scrollbar
    anchors.right: flickable.right
    anchors.top: flickable.top
    anchors.bottom: flickable.bottom
    width: 10
    color: "#CCCCCC"
    visible: flickable.contentHeight > flickable.height
    Rectangle {
      anchors { left: parent.left; right: parent.right }
      y: flickable.visibleArea.yPosition * flickable.height
      height: flickable.visibleArea.heightRatio * flickable.height
      color: "#3E65FF"
    }
  }
}
