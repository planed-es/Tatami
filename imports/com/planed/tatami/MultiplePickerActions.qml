import QtQuick 2.12

ActionSet {
  actions: [confirmAction, switchToOptionsAction, switchToSelectionAction, addAction, removeAction]

  signal confirmed()

  Action {
    id: confirmAction
    text: i18n.t("actions.confirm")
    icon.name: "dialog-ok"
    sequence: "F5"
    onTriggered: confirmed()
  }

  Action {
    id: switchToOptionsAction
    sequence: "F1"
    icon.name: "list-add"
    text: i18n.t("actions.pick")
    enabled: !optionPicker.hasFocus
    onTriggered: view.switchMode()
  }

  Action {
    id: switchToSelectionAction
    sequence: "F1"
    icon.name: "view"
    text: i18n.t("actions.selection")
    enabled: optionPicker.hasFocus
    onTriggered: view.switchMode()
  }

  Action {
    id: addAction
    sequence: "F2"
    icon.name: "list-add"
    text: i18n.t("actions.add")
    enabled: optionPicker.hasFocus
    onTriggered: view.addCurrentOption()
  }

  Action {
    id: removeAction
    sequence: "F2"
    icon.name: "list-remove"
    text: i18n.t("actions.remove")
    enabled: !optionPicker.hasFocus
    onTriggered: view.removeCurrentOption()
  }
}
