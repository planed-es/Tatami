import QtQuick 2.12
import QtQuick.Controls 2.12

ActionSet {
  property QtObject model:        view.model
  property string   propertyName: view.propertyName
  property var      value:        view.value

  actions: [confirmAction, switchToOptionsAction, switchToSelectionAction, addAction, removeAction]

  Action {
    id: confirmAction
    text: i18n.t("actions.confirm")
    icon.name: "dialog-ok"
    shortcut: Shortcut {
      sequence: "F5"
      onActivated: confirmAction.trigger()
    }
    onTriggered: {
      model[propertyName] = value;
      application.closeCurrentView();
    }
  }

  Shortcut {
    id: switchShortcut
    sequence: "F1"
    onActivated: {
      if (switchToOptionsAction.enabled)
        switchToOptionsAction.trigger();
      else
        switchToSelectionAction.trigger();
    }
  }

  Shortcut {
    id: actionShortcut
    sequence: "F2"
    onActivated: {
      if (removeAction.enabled)
        removeAction.trigger();
      else
        addAction.trigger();
    }
  }

  Action {
    id: switchToOptionsAction
    icon.name: "list-add"
    text: i18n.t("actions.pick")
    enabled: !optionPicker.hasFocus
    shortcut: switchShortcut
    onTriggered: root.switchMode()
  }

  Action {
    id: switchToSelectionAction
    icon.name: "view"
    text: i18n.t("actions.selection")
    enabled: optionPicker.hasFocus
    shortcut: switchShortcut
    onTriggered: root.switchMode()
  }

  Action {
    id: addAction
    icon.name: "list-add"
    text: i18n.t("actions.add")
    enabled: optionPicker.hasFocus
    shortcut: actionShortcut
    onTriggered: root.addCurrentOption()
  }

  Action {
    id: removeAction
    icon.name: "list-remove"
    text: i18n.t("actions.remove")
    enabled: !optionPicker.hasFocus
    shortcut: actionShortcut
    onTriggered: root.removeCurrentOption()
  }
}
