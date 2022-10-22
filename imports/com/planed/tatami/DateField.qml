import QtQuick 2.12

TextField {
  property date date: new Date()

  signal valueChanged();

  function selectNextPart() {
    if (cursorPosition == 2)
      select(3, 5);
    else if (cursorPosition == 5)
      select(6, 10);
    else if (cursorPosition == 10)
      select(0, 2);
    else
      return false;
    return true;
  }

  function selectPreviousPart() {
    if (cursorPosition == 3) {
      cursorPosition = 0;
      select(2, 0);
    } else if (cursorPosition == 0) {
      cursorPosition = 6;
      select(10, 6);
    } else if (cursorPosition == 6) {
      cursorPosition = 3;
      select(5, 3);
    } else
      return false;
    return true;
  }

  function fixInvalidDate() {
    var match;

    if ((match = text.match(/^([0-9]{0,1})(-[0-9]{2}-[0-9]{4})$/)))
      text = "0" + (match[1].length > 0 ? match[1] : '1') + match[2];
    else if ((match = text.match(/^([0-9]{2}-)([0-9]{0,1})(-[0-9]{4})$/)))
      text = match[1] + "0" + (match[2].length > 0 ? match[2] : "1") + match[3];
    else if ((match = text.match(/^([0-9]{2}-[0-9]{2}-)([0-9]{0,3})$/))) {
      var mytext = match[1] + "2";
      if (match[2].length < 3)
        mytext += "0";
      if (match[2].length < 2)
        mytext += "2";
      if (match[2].length < 1)
        mytext += "1";
      mytext += match[2];
      text = mytext;
    }
  }

  onTextChanged: {
    const asDate = Date.fromLocaleDateString(Qt.locale(), text, "dd-MM-yyyy")

    if (asDate !== date && asDate.toLocaleDateString() !== "") {
      date = asDate;
      valueChanged();
    }
    if (activeFocus)
      selectNextPart();
  }

  onDateChanged: {
    const asText = date.toLocaleDateString(Qt.locale(), "dd-MM-yyyy");

    if (asText !== text) {
      text = asText;
      dateChanged();
    }
  }

  onActiveFocusChanged: {
    if (activeFocus)
      selectNextPart();
  }

  Keys.onRightPressed: {
    fixInvalidDate();
    if (!selectNextPart()) {
      select(cursorPosition, cursorPosition + 1);
    }
  }

  Keys.onLeftPressed: {
    fixInvalidDate();
    if (!selectPreviousPart())
      select(cursorPosition, cursorPosition - 1);
  }
}
