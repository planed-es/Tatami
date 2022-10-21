import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
  id: root
  property var table
  property var actions: [moveUpAction, moveDownAction, activateAction]

  onEnabledChanged: {
    actions.forEach(action => { action.enabled = enabled; });
  }

  Action {
    id: moveUpAction
    shortcut: Shortcut {
      sequence: "Up"
      enabled: moveUpAction.enabled
      onActivated: moveUpAction.trigger()
    }
    onTriggered: table.decrementIndex()
  }

  Action {
    id: moveDownAction
    shortcut: Shortcut {
      sequence: "Down"
      enabled: moveDownAction.enabled
      onActivated: moveDownAction.trigger()
    }
    onTriggered: table.incrementIndex()
  }

  Action {
    id: activateAction
    shortcut: Shortcut {
      sequences: ["Enter", "Return"]
      enabled: activateAction.enabled
      onActivated: activateAction.trigger()
    }
    onTriggered: table.activated(table.currentIndex, table.model.modelAt(table.currentIndex))
  }
}
