import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Pane {
  id: form
  property alias content: formLayout.children
  property var inputMargin: 10
  property var minInputWidth: 300
  property var columnCount: width / (minInputWidth + inputMargin)
  property var columnWidth: width / formLayout.columns
  property real computedInputWidth: form.columnWidth - 2 * form.inputMargin
  property bool canSubmit: true
  property alias inputControl: inputControlLoader.sourceComponent
  property var currentInput: null

  signal submitted();

  function submit() {
    if (canSubmit)
      submitAction.trigger();
  }

  onWidthChanged: {
    columnCount = width / (minInputWidth + inputMargin);
    columnWidth = Math.max(width / formLayout.columns, minInputWidth);
  }

  onContentChanged: {
    for (var i = 0 ; i < content.length ; ++i) {
      content[i].form = form;
      content[i].nextInput = content[i + 1];
    }
  }

  onCurrentInputChanged: {
    if (currentInput !== null && currentInput.inputControl) {
      console.log("Haz inputcontrol", currentInput.inputControl);
      inputControl = currentInput.inputControl;
    }
    else
      inputControl = null;
  }

  height: formLayout.height + formControls.height

  Action {
    id: submitAction
    text: qsTr("Submit")
    icon.name: "list-add"
    onTriggered: form.submitted()
  }

  GridLayout {
    id: formLayout
    anchors.left: parent.left
    anchors.right: parent.right
    columns: Math.max(1, form.columnCount)
  }

  Loader {
    id: inputControlLoader
    anchors.topMargin: 10
    anchors.bottomMargin: 10
    anchors.top: formLayout.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: formControls.top

    onStatusChanged: {
      console.log(">> Loader state changed to", status);
    }

    onItemChanged: {
      console.log(">> Item has changed", item, width, height, sourceComponent, item.height, getLeftoverHeight());
    }

    function getLeftoverHeight() {
      return form.height - (formLayout.height + formControls.height);
    }
  }

  RowLayout {
    id: formControls
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    height: 75

    ToolButton {
      Layout.alignment: Qt.AlignRight
      action: submitAction
      enabled: canSubmit
    }
  }
}
