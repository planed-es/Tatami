import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

QQControls.Dialog {
  property var actions: dialogActions
  property var parentView
  property var fallbackActionSet

  id: root
  standardButtons: QQControls.Dialog.Ok
  parent: QQControls.Overlay.overlay
  x: parent.x
  width: parent.width
  y: parent.height - height
  modal: false
  closePolicy: QQControls.Popup.CloseOnEscape

  onOpened: {
    application.modal = this;
    forceActiveFocus();
    if (parentView) {
      fallbackActionSet = parentView.viewActionsComponent;
      parentView.viewActionsComponent = actions;
    }
    standaloneActions.sourceComponent = actions;
  }

  onClosed: {
    application.modal = null;
    if (parentView) {
      parentView.viewActionsComponent = fallbackActionSet;
      fallbackActionSet = null;
    }
    standaloneActions.sourceComponent = null;
  }

  Loader {
    id: standaloneActions
  }

  Component {
    id: dialogActions

    ActionSet {
      actions: [confirmAction]
      view: parentView
      backEnabled: false

      Action {
        id: confirmAction
        text: i18n.t("Ok") + " (F5)"
        icon.name: "dialog-ok"
        sequence: "F5"
        onTriggered: root.accept()
      }
    }
  }
}

