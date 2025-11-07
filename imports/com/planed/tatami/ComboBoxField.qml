import QtQuick 2.12
import QtQuick.Controls 2.12
import "." as Tatami

Tatami.PickerField {
  id: root
  property var value
  property var options: []
  readonly property bool readOnly: !enabled
  readonly property var currentIndex: {
    for (let i = 0 ; i < options.length ; ++i) {
      if (options[i].value == value)
        return i;
    }
    return -1;
  }

  Keys.onDownPressed: selectNextValue()
  Keys.onUpPressed: selectPreviousValue()
  displayFunction: function() {
    return options[currentIndex].label;
  }

  function selectNextValue() {
    if (currentIndex + 1 < options.length)
      root.model[propertyName] = options[currentIndex + 1].value;
  }

  function selectPreviousValue() {
    if (currentIndex > 0)
      root.model[propertyName] = options[currentIndex - 1].value;
  }
}

