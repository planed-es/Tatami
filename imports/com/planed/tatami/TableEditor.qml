import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls
import "./TableHelpers.js" as Helpers
import "." as Tatami
import com.planed.tatami.helpers

Tatami.TableView {
  id: root
  property QtObject sample
  property var inputs: []
  property var parentView
  property alias fields: fieldsLoader.item
  property alias fieldsComponent: fieldsLoader.sourceComponent
  property bool withConfirmDialog: true
  property bool activationEnabled: true

  cellComponentProvider: fields.componentProvider
  navigationEnabled: model.state === TableModel.ReadState
  actionComponent:   model.state === TableModel.ReadState ? null : editActions

  signal accepted()
  signal canceled()

  function takeFocus() { fields.takeFocus(); }

  Connections {
    target: model
    function onStateChanged() {
      fields.model = root.sample;
      fields.reset();
      reloadRow(model.currentIndex);
      if (model.state !== TableModel.ReadState)
        delayedFieldFocus.running = true;
    }
    function onCurrentIndexChanged() {
      if (model.state !== TableModel.ReadState)
        delayedFocusRetriever.running = true;
    }
  }

  Timer {
    id: delayedFieldFocus
    interval: 5; onTriggered: root.takeFocus()
  }

  Timer {
    id: delayedFocusRetriever
    interval: 5; onTriggered: fields.retakeFocus();
  }

  Loader {
    id: fieldsLoader
    sourceComponent: TableFields {
      table: root
    }
    onLoaded: item.table = root
  }

  ConfirmDialog {
    id: confirmDialog
    title: "¿Guardar cambios?"
    parentView: root.parentView
    onAccepted: root.accepted()
    onRejected: root.canceled()
  }

  Component {
    id: editActions
    ActionSet {
      actions: [saveAction, cancelAction]
      view: root.parentView
      backEnabled: false

      QQControls.Action {
        id: validateAction
        text: i18n.t("actions")
        shortcut: Shortcut {
          sequences: ["Enter", "Return"]
          enabled: validateAction.enabled
          onActivated: validateAction.trigger()
        }
        onTriggered: {
          if (fields.lastField().activeFocus) {
            if (withConfirmDialog)
              confirmDialog.open();
            else
              root.accepted();
          }
          else if (fields.getCurrentField() === undefined)
            root.accepted(); // there is nothing to save, but this will make the cursor move to the next line
          else
            fields.getCurrentField().accepted();
        }
      }

      QQControls.Action {
        id: saveAction
        text: i18n.t("actions.save")
        icon.name: "media-floppy"
        shortcut: Shortcut {
          sequence: "F5"
          onActivated: saveAction.trigger()
        }
        onTriggered: root.accepted()
      }

      QQControls.Action {
        id: cancelAction
        text: i18n.t("Cancel")
        icon.name: "dialog-ko"
        shortcut: Shortcut {
          sequence: "F4"
          onActivated: cancelAction.trigger()
        }
        onTriggered: root.canceled()
      }
    }
  }
}
