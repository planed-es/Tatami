import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "." as Tatami

Tatami.View {
  id: root
  viewActionsComponent: defaultActions
  property QtObject tableStyle: Tatami.TableStyle {}
  property alias    optionPicker: optionPicker
  property alias    controller: optionPicker.controller
  property QtObject selectionController
  property var      selectionFilter: ({ "uid": "(" + root.controller.value.join("|") + ")" })
  property var      value
  property var      callback
  property var      columns
  property string   mainColumn
  property bool     accepted: false

  Component.onCompleted: {
    root.controller.value = root.value;
    root.value = Qt.binding(function() { return root.controller.value; });
  }

  function removeUid(uid) {
    const newList = [];

    for (var i = 0 ; i < root.value.length ; ++i) {
      if (root.value[i].toString() !== uid.toString())
        newList.push(root.value[i]);
    }
    root.controller.value = newList;
  }

  function addUid(uid) {
    const newList = root.value;

    newList.push(uid);
    root.controller.value = newList;
  }

  function removeCurrentOption() {
    removeUid(selectionController.table.modelAt(selectionTable.currentIndex).uid);
  }

  function addCurrentOption() {
    addUid(optionPicker.controller.models[optionPicker.currentIndex].uid);
  }

  function switchMode() {
    (optionPicker.hasFocus ? selectionTable : optionPicker).forceActiveFocus();
  }

  function save() {
    if (callback)
      callback(value);
  }

  Component.onDestruction: {
    if (accepted)
      save();
  }

  Component {
    id: defaultActions
    Tatami.MultiplePickerActions {
      view: root
      onConfirmed: {
        root.accepted = true;
        application.closeCurrentView();
      }
    }
  } // END ActionSet

  Connections {
    target: selectionController
    function onModelsChanged() {
      selectionController.table.setColumns(root.columns);
    }
  }

  Connections {
    target: root.controller
    function onValueChanged() {
      selectionController.filters = root.selectionFilter;
    }
  }

  onColumnsChanged: {
    selectionController.table.setColumns(columns);
    optionPicker.controller.table.setColumns(columns);
  }

  Rectangle {
    id: focusHint
    width: 5
    color: "#3E65FF"
  }

  Item {
    id: stateHolder

    states: [
      State {
        name: "selection"; when: !optionPicker.hasFocus
        PropertyChanges { target: focusHint; y: titleSelection.y; height: selectionTable.height + titleSelection.height + 5 }
      },
      State {
        name: "picking";   when: optionPicker.hasFocus
        PropertyChanges { target: focusHint; y: titleOptions.y; height: optionPicker.height + titleOptions.height }
      }
    ]

    transitions: [
      Transition {
        to: "selection"
        PropertyAnimation { target: focusHint; property: "height"; from: titleOptions.height + optionPicker.height }
        PropertyAnimation { target: focusHint; property: "y";      from: titleOptions.y }
        PropertyAnimation { target: titleSelection; property: "bgColor"; to: root.tableStyle.selectionColor }
        PropertyAnimation { target: titleOptions; property: "bgColor"; to: root.tableStyle.headerBackgroundColor }
      },
      Transition {
        to: "picking"
        PropertyAnimation { target: focusHint; property: "height"; from: selectionTable.height + titleSelection.height + 5 }
        PropertyAnimation { target: focusHint; property: "y";      from: titleSelection.y }
        PropertyAnimation { target: titleSelection; property: "bgColor"; to: root.tableStyle.headerBackgroundColor }
        PropertyAnimation { target: titleOptions; property: "bgColor"; to: root.tableStyle.selectionColor }
      }
    ]
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.leftMargin: focusHint.width

    Label {
      property color bgColor: root.tableStyle.headerBackgroundColor
      id: titleSelection
      text: i18n.t("model-list-picker.value")
      Layout.fillWidth: true
      font.capitalization: Font.Capitalize
      font.weight: optionPicker.hasFocus ? Font.Normal : Font.Bold
      font.pixelSize: 20
      padding: 10
      background: Rectangle { color: titleSelection.bgColor }
    }

    Tatami.TableView {
      id: selectionTable
      model: selectionController.table
      Layout.fillWidth: true
      Layout.preferredHeight: Math.min(root.height / 2, root.value.length * 50 + 35)
      navigationEnabled: !optionPicker.hasFocus
      mainColumn: root.mainColumn

      onActivated: {
        removeUid(model.uid);
      }

      Rectangle {
        anchors.fill: parent
        color: Qt.rgba(50, 50, 50, 0.5)
      }
    }

    Item {
      implicitHeight: 20
      Layout.fillWidth: true
    }

    Label {
      property color bgColor: root.tableStyle.headerBackgroundColor
      id: titleOptions
      text: i18n.t("model-list-picker.options")
      Layout.fillWidth: true
      font.capitalization: Font.Capitalize
      font.weight: optionPicker.hasFocus ? Font.Bold : Font.Normal
      font.pixelSize: 20
      padding: 10
      background: Rectangle { color: titleOptions.bgColor }
    }

    Tatami.TablePicker {
      id: optionPicker
      Layout.fillHeight: true
      Layout.fillWidth: true
      mainColumn: root.mainColumn
      onPicked: addUid(model.uid)
    }
  }
}
