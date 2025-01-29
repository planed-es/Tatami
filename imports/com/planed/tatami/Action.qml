import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls

QQControls.Action {
  property string sequence
  property var sequences: []
  property QtObject actionSet
  property bool autoRepeat: false
  property bool hidden: false
  id: action
  shortcut: Shortcut {
    context: Qt.WindowShortcut
    autoRepeat: action.autoRepeat
    enabled: action.enabled && (!actionSet || actionSet.enabled)
    sequence: enabled ? action.sequence : ""
    sequences: enabled ? action.sequences : []
    onActivated: {
      action.trigger();
    }
    onActivatedAmbiguously: {
      if (actionSet && actionSet.enabled)
        action.trigger();
    }
  }
}
