import QtQuick 2.12

Item {
  id: root
  property var model
  property var fields: []
  property bool hasFocus: false
  property int currentField: -1
  property int focusBackup: -1
  property var fieldAfter: null
  property var customFields: ({})

  property alias textEditCell: textEditCell
  property alias numberEditCell: numberEditCell
  property alias boolEditCell: boolEditCell
  property alias dateEditCell: dateEditCell

  function reset() {
    fields = [];
    hasFocus = false;
  }

  function appendField(field, colIndex) {
    const tmp = fields;
    while (colIndex >= tmp.length)
      tmp.push(null);
    tmp[colIndex] = field;
    fields = tmp;
  }

  function takeFocus() {
    var firstField = nextField(fields, -1);

    if (firstField)
      firstField.forceActiveFocus();
  }

  function retakeFocus() {
    var field = fields[focusBackup];

    if (field)
      field.forceActiveFocus();
    else
      takeFocus();
  }

  function getCurrentField() {
    return fields[currentField];
  }

  function lastField() {
    if (fieldAfter !== null)
      return fieldAfter;
    for (var i = fields.length - 1 ; i >= 0 ; --i) {
      if (fields[i] != null && !fields[i].readOnly)
        return fields[i];
    }
    return null;
  }

  function fieldFor(propertyName) {
    return defaultFieldFor(propertyName);
  }

  function defaultFieldFor(propertyName) {
    if (customFields[propertyName] !== undefined)
      return customFields[propertyName];
    switch (typeof root.model[propertyName]) {
    case "boolean":
      return boolEditCell;
    case "number":
      return numberEditCell;
    case "string":
      return textEditCell;
    }
    if (root.model[propertyName] && root.model[propertyName].constructor) {
      switch (root.model[propertyName].constructor.name) {
      case "Date":
        return dateEditCell;
      case "ArrayBuffer":
        return textEditCell;
      }
    }
    return null;
  }

  function updateFocus(a) {
    for (var i = 0 ; i < fields.length ; ++i) {
      if (fields[i] && fields[i].activeFocus) {
        focusBackup = i;
        currentField = i;
        hasFocus = true;
        return ;
      }
    }
    currentField = -1;
    hasFocus = false;
  }

  function clearFields() {
    for (var i = 0 ; i < fields.length ; ++i) {
      if (fields[i] && fields[i].text)
        fields[i].text = "";
    }
  }

  function nextField(inputs, colIndex, isActive = true) {
    const match = function(input) {
      return input != null && input.enabled && !input.readOnly;
    };

    if (!isActive)
      return null;
    return inputs.slice(colIndex + 1, inputs.length).find(match)
        || fieldAfter
        || inputs.slice(0, colIndex).find(match)
        || inputs[colIndex]
        || this;
  }

  function onValueChanged(propertyName, value) {
    root.model[propertyName] = value;
  }

  function goToView(colIndex, view, properties) {
    const callback = properties.callback;

    properties.callback = function() {
      onFieldViewClosed(colIndex);
      if (typeof callback == "function")
        callback();
    };
    application.goToView(view, properties);
  }

  function onFieldViewClosed(colIndex) {
    focusRestitutionTimer.colIndex = colIndex;
    focusRestitutionTimer.running = true;
  }

  Timer {
    property int colIndex
    id: focusRestitutionTimer
    interval: 250
    onTriggered: {
      console.log("Tatami.Fields giving back focus", colIndex, root.fields[colIndex]);
      root.fields[colIndex].forceActiveFocus();
      if (lastField() != root.fields[colIndex])
        root.fields[colIndex].accepted();
    }
  }

  Component {
    id: textEditCell
    TextField {
      text: value
      onTextChanged: root.onValueChanged(propertyName, text)
      Component.onCompleted: root.appendField(this, colIndex)
      KeyNavigation.tab: root.nextField(root.fields, colIndex)
      onActiveFocusChanged: root.updateFocus()
    }
  }

  Component {
    id: numberEditCell
    NumberField {
      number: parseFloat(value)
      decimals: 3
      onNumberChanged: root.onValueChanged(propertyName, number)
      Component.onCompleted: root.appendField(this, colIndex)
      KeyNavigation.tab: root.nextField(root.fields, colIndex)
      onActiveFocusChanged: root.updateFocus()
    }
  }

  Component {
    id: boolEditCell
    BooleanField {
      checked: value
      onCheckedChanged: root.onValueChanged(propertyName, checked)
      Component.onCompleted: root.appendField(this, colIndex)
      KeyNavigation.tab: root.nextField(root.fields, colIndex)
      onActiveFocusChanged: root.updateFocus()
    }
  }

  Component {
    id: dateEditCell
    DateField {
      date: model[propertyName]
      onValueChanged: root.onValueChanged(propertyName, date)
      Component.onCompleted: root.appendField(this, colIndex)
      KeyNavigation.tab: root.nextField(root.fields, colIndex)
      onActiveFocusChanged: root.updateFocus()
    }
  }
}
