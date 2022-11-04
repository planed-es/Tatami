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

      Action {
        id: confirmAction
        text: i18n.t("Ok")
        icon.name: "dialog-ok"
        sequence: "F5"
        onTriggered: confirmDialog.accept()
      }

      Action {
        id: cancelAction
        text: i18n.t("Cancel")
        icon.name: "dialog-ko"
        sequence: "F4"
        onTriggered: confirmDialog.reject()
      }
    }
  }
}
