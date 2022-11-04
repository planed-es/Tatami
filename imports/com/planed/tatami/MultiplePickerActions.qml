import QtQuick 2.12

ActionSet {
  property QtObject model:        view.model
  property string   propertyName: view.propertyName
  property var      value:        view.value

  actions: [confirmAction, switchToOptionsAction, switchToSelectionAction, addAction, removeAction]

  Action {
    id: confirmAction
    text: i18n.t("actions.confirm")
    icon.name: "dialog-ok"
    sequence: "F5"
    onTriggered: {
      model[propertyName] = value;
      application.closeCurrentView();
    }
  }

  Action {
    id: switchToOptionsAction
    sequence: "F1"
    icon.name: "list-add"
    text: i18n.t("actions.pick")
    enabled: !optionPicker.hasFocus
    shortcut: switchShortcut
    onTriggered: root.switchMode()
  }

  Action {
    id: switchToSelectionAction
    sequence: "F1"
    icon.name: "view"
    text: i18n.t("actions.selection")
    enabled: optionPicker.hasFocus
    shortcut: switchShortcut
    onTriggered: root.switchMode()
  }

  Action {
    id: addAction
    sequence: "F2"
    icon.name: "list-add"
    text: i18n.t("actions.add")
    enabled: optionPicker.hasFocus
    shortcut: actionShortcut
    onTriggered: root.addCurrentOption()
  }

  Action {
    id: removeAction
    sequence: "F2"
    icon.name: "list-remove"
    text: i18n.t("actions.remove")
    enabled: !optionPicker.hasFocus
    shortcut: actionShortcut
    onTriggered: root.removeCurrentOption()
  }
}
