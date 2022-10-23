import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls
import QtQuick.Layouts 1.12
import com.planed.tatami 1.0

Dialog {
  property string message
  property string icon: "security-medium"

  height: dialogView.height + 150

  RowLayout {
    id: dialogView
    anchors.horizontalCenter: parent.horizontalCenter

    Image {
      source: Icons.url(icon, 48)
    }

    QQControls.Label {
      Layout.fillWidth: true
      text: message
    }
  }
}
