import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
  property string label
  property Component field
  property int minWidth: 100
  property bool hasFocus: fieldLoader.item.activeFocus

  SystemPalette {
    id: colors
    colorGroup: parent.enabled ? SystemPalette.Active : SystemPalette.Disabled
  }

  Text {
    Layout.fillWidth: true
    horizontalAlignment: Qt.AlignHCenter
    text: label
    color: parent.enabled ? colors.text : "gray"
    font.capitalization: Font.Capitalize
    font.underline: fieldLoader.item.activeFocus
  }

  Loader {
    id: fieldLoader
    Layout.preferredWidth: minWidth
    Layout.fillWidth: true
    sourceComponent: field
  }

  onActiveFocusChanged: fieldLoader.item.forceActiveFocus()
}
