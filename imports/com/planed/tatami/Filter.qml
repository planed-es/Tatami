import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
  id: root
  property QtObject model
  property var propertyList
  property alias hasFocus: fieldController.hasFocus

  signal filterUpdate(var filterData)

  function takeFocus() {
    fieldController.fields[0].forceActiveFocus();
  }

  function makeData() {
    var filter = {};

    for (var i = 0 ; i < propertyList.length ; ++i) {
      const propertyName = propertyList[i];

      filter[propertyName] = model[propertyName];
    }
    return filter;
  }

  function clear() {
    fieldController.clearFields();
  }

  Connections {
    target: model
    function onAttributeChanged() {
      root.filterUpdate(root.makeData());
    }
  }

  RowLayout {
    spacing: 10
    anchors { left: parent.left; right: parent.right }

    Image {
      Layout.alignment: Qt.AlignRight
      source: "qrc:/icons/icon-theme/48x48/edit-find.png"
    }

    Repeater {
      model: propertyList
      delegate: ColumnLayout {
        Text {
          Layout.fillWidth: true
          horizontalAlignment: Qt.AlignHCenter
          text: i18n.t("properties." + propertyList[index])
          font.capitalization: Font.Capitalize
        }

        Loader {
          property string propertyName: root.propertyList[index]
          property variant value: root.model[propertyName]
          property int colIndex: index
          sourceComponent: fieldController.fieldFor(propertyName)
          Layout.preferredWidth: 100
          Layout.fillWidth: true
        }
      }
    }
  }

  Fields {
    id: fieldController
    model: root.model
  }
}
