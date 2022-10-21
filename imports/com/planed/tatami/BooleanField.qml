import QtQuick 2.12
import QtQuick.Controls 2.12

CheckBox {
  signal accepted();
  Keys.onReturnPressed: accepted()
  Keys.onEnterPressed: accepted()
  onAccepted: (KeyNavigation.tab || nextItemInFocusChain()).forceActiveFocus()
}
