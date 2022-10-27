import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

QQControls.Action {
  property string sequence
  id: action
  shortcut: Shortcut {
    id: shortcut
    context: Qt.ApplicationShortcut
    sequence: action.sequence
    enabled: action.enabled
    onActivated: action.trigger()
  }
}
