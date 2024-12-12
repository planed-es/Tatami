import QtQuick.Controls 2.12 as QQControls
import QtQuick.Layouts 1.12
import com.planed.tatami 1.0

ConfirmDialog {
  property alias label: labelComponent.text
  property alias validator: promptInput.validator
  property alias value: promptInput.text
  onActiveFocusChanged: if (activeFocus) {
    promptInput.forceActiveFocus();
  }
  RowLayout {
    anchors.horizontalCenter: parent.horizontalCenter
    QQControls.Label { id: labelComponent }
    TextField { id: promptInput }
  }
}
