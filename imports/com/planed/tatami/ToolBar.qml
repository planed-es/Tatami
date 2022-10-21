import QtQuick 2.12
import QtQuick.Controls 2.5 as QQControls
import QtQuick.Layouts 1.3

QQControls.ToolBar {
  property int viewDepth
  property var viewActions: []
  property bool backEnabled: true
  property alias exitAction: exitAction
  height: 60
  clip: true

  QQControls.Action {
    id: exitAction
    text: i18n.t("actions.back")
    icon.name: "go-previous"
    enabled: backEnabled && viewDepth > 1
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
        visible: enabled
      }
    }

    Item {
      Layout.fillWidth: true
    }

    QQControls.ToolSeparator {}

    QQControls.Label {
      Layout.alignment: Qt.AlignRight
      Layout.rightMargin: 10
      Timer {
        interval: 1000; running: true; repeat: true;
        onTriggered: {
          var date = new Date();
          parent.text = date.toLocaleString()
        }
      }
    }
  }

  Loader {
    sourceComponent: exitAction.enabled ? backComponent : null
  }

  Component {
    id: backComponent
    Item {
      Shortcut {
        sequence: "F4"
        onActivated: exitAction.triggered()
        Component.onCompleted: exitAction.shortcut = this;
        Component.onDestruction: exitAction.shortcut = null;
      }
    }
  }
}
