import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
  property string label
  property Component field
  property int minWidth: 100
  property bool hasFocus: fieldLoader.item.activeFocus

  Text {
    Layout.fillWidth: true
    horizontalAlignment: Qt.AlignHCenter
    text: label
    font.capitalization: Font.Capitalize
  }

  Loader {
    id: fieldLoader
    Layout.preferredWidth: minWidth
    Layout.fillWidth: true
    sourceComponent: field
  }

  onActiveFocusChanged: fieldLoader.item.forceActiveFocus()
}
