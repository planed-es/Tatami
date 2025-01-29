import QtQuick 2.12
import QtQuick.Controls 2.5 as QQControls
import QtQuick.Layouts 1.3

QQControls.ToolBar {
  property int viewDepth
  property var viewActions: []
  property bool backEnabled: true
  property alias exitAction: exitAction
  property Component additionalWidget
  height: 60
  clip: true

  Action {
    id: exitAction
    text: i18n.t("actions.back")
    icon.name: "go-previous"
    enabled: backEnabled && viewDepth > 1
    sequence: "F4"
    onTriggered: application.closeCurrentView()
  }

  RowLayout {
    anchors.fill: parent

    ToolButton {
      Layout.alignment: Qt.AlignLeft
      action: exitAction
      enabled: exitAction.enabled
    }

    QQControls.ToolSeparator {}

    Repeater {
      model: viewActions
      delegate: ToolButton {
        Layout.alignment: Qt.AlignLeft
        action: model.modelData
        visible: enabled && viewActions[index].hidden !== true
      }
    }

    Item {
      Layout.fillWidth: true
    }

    QQControls.ToolSeparator {
      visible: additionalWidget != null
    }

    Loader {
      Layout.alignment: Qt.AlignRight
      Layout.rightMargin: 10
      sourceComponent: additionalWidget
    }
  }
}
