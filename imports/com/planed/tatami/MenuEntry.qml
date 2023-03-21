import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

QQControls.Action {
  id: action
  property string sequence
  property QtObject parentView: action.parent

  shortcut: Shortcut {
    context: Qt.ApplicationShortcut
    sequence: action.sequence
    enabled: parentView ? parentView.isActiveView : false
    onActivated: action.trigger()
  }
}
