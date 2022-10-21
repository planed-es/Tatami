import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

Dialog {
  id: confirmDialog
  standardButtons: QQControls.Dialog.Ok | QQControls.Dialog.Cancel
  actions: confirmDialogActions

  Component {
    id: confirmDialogActions

    ActionSet {
      actions: [confirmAction, cancelAction]
      view: parentView
      backEnabled: false

      QQControls.Action {
        id: confirmAction
        text: i18n.t("Ok") + " (F5)"
        icon.name: "dialog-ok"
        onTriggered: confirmDialog.accept()
      }

      QQControls.Action {
        id: cancelAction
        text: i18n.t("Cancel") + " (F4)"
        icon.name: "dialog-ko"
        onTriggered: confirmDialog.reject()
      }

      Shortcut {
        sequence: "F5"
        onActivated: confirmAction.trigger()
      }

      Shortcut {
        sequence: "F4"
        onActivated: cancelAction.trigger()
      }
    }
  }
}
