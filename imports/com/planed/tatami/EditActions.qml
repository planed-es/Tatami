import QtQuick 2.12
import QtQuick.Controls 2.12

ActionSet {
  actions: [validateAction, cancelAction]
  backEnabled: false

  signal persist();
  signal cancel();

  Action {
    id: validateAction
    text: i18n.t("actions.save")
    icon.name: "media-floppy"
    shortcut: Shortcut {
      sequence: "F5"
      onActivated: validateAction.trigger()
    }
    onTriggered: persist()
  }

  Action {
    id: cancelAction
    text: i18n.t("Cancel")
    icon.name: "dialog-ko"
    shortcut: Shortcut {
      sequence: "F4"
      onActivated: cancelAction.trigger()
    }
    onTriggered: cancel();
  }
}
