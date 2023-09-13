import QtQuick 2.12
import "./TableHelpers.js" as Helpers
import com.planed.tatami 1.0 as Tatami

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
  navigationEnabled: model.state === Tatami.TableModel.ReadState
  actionComponent:   model.state === Tatami.TableModel.ReadState ? null : editActions

  signal accepted()
  signal canceled()

  function takeFocus() { fields.takeFocus(); }

  Connections {
    target: model
    function onStateChanged() {
      fields.model = root.sample;
      fields.reset();
      reloadRow(model.currentIndex);
      if (model.state !== Tatami.TableModel.ReadState)
        delayedFieldFocus.running = true;
    }
    function onCurrentIndexChanged() {
      if (model.state !== Tatami.TableModel.ReadState)
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

  Timer {
    id: acceptLimit
    interval: 250
    repeat: false
    running: false
  }

  function acceptFunnel() {
    if (!acceptLimit.running) {
      acceptLimit.restart();
      root.accepted();
    }
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

      Action {
        id: validateAction
        text: i18n.t("actions")
        sequences: ["Enter", "Return"]
        onTriggered: {
          if (fields.lastField().activeFocus) {
            if (withConfirmDialog)
              confirmDialog.open();
            else
              acceptFunnel();
          }
          else if (fields.getCurrentField() === undefined)
            acceptFunnel();
          else
            fields.getCurrentField().accepted();
        }
      }

      Action {
        id: saveAction
        text: i18n.t("actions.save")
        icon.name: "media-floppy"
        sequence: "F5"
        onTriggered: root.accepted()
      }

      Action {
        id: cancelAction
        text: i18n.t("Cancel")
        icon.name: "dialog-ko"
        sequence: "F4"
        onTriggered: root.canceled()
      }
    }
  }
}
