import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

QQControls.Dialog {
  property var actions: dialogActions
  property var parentView
  property var fallbackActionSet
  property var actionList: standaloneActions.item ? standaloneActions.item.actions : []

  id: root
  standardButtons: QQControls.Dialog.Ok
  parent: QQControls.Overlay.overlay
  x: parent.x
  width: parent.width
  y: parent.height - height
  modal: false
  closePolicy: QQControls.Popup.CloseOnEscape

  function attachActions() {
    if (parentView) {
      fallbackActionSet = parentView.viewActionsComponent;
      parentView.viewActionsComponent = actions;
    }
  }

  function detachActions() {
    if (parentView) {
      parentView.viewActionsComponent = fallbackActionSet;
      fallbackActionSet = null;
    }
  }

  onOpened: {
    application.modal = this;
    forceActiveFocus();
    attachActions();
    standaloneActions.sourceComponent = actions;
  }

  onClosed: {
    application.modal = null;
    detachActions();
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

