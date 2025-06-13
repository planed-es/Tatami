import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "." as Tatami

Tatami.Dialog {
  property alias content: dialogContent.sourceComponent

  id: dialogRoot
  standardButtons: Dialog.NoButton
  height: dialogView.height + 50

  ColumnLayout {
    id: dialogView
    anchors.horizontalCenter: parent.horizontalCenter

    Loader {
      id: dialogContent
    }

    RowLayout {
      Repeater {
        model: dialogRoot.actionList
        delegate: Button {
          action: dialogRoot.actionList[index]
        }
      }
    }
  }
}
