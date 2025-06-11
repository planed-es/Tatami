import QtQuick 2.12

Fields {
  id: root
  property var table

  fields: table.inputs
  model: table.sample

  function componentProvider(row, column, propertyName) {
    if (table.model.isEditableRow(row))
      return fieldFor(propertyName);
    return null;
  }

  function goToView(colIndex, view, properties) {
    const callbackParam = properties.callback;

    properties.callback = function() {
      onFieldViewClosed(colIndex);
      if (typeof callbackParam == "function")
        callbackParam();
    };
    application.goToView(view, properties);
  }
}
