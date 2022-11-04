import QtQuick 2.12

ActionSet {
  actions: [validateAction, cancelAction]
  backEnabled: false

  signal persist();
  signal cancel();

  Action {
    id: validateAction
    text: i18n.t("actions.save")
    icon.name: "media-floppy"
    sequence: "F5"
    onTriggered: persist()
  }

  Action {
    id: cancelAction
    text: i18n.t("Cancel")
    icon.name: "dialog-ko"
    sequence: "F4"
    onTriggered: cancel();
  }
}
