import QtQuick 2.12

PaginatedActions {
  property bool withAdd: true
  property bool withFilters: false
  property bool withRemove: false
  property bool withPrint: false

  function labelForAction(actionName) {
    return i18n.t(actionName, { resourceName: i18n.t("model." + view.resourceName) });
  }

  function prepareFiltersAction(array) {
    array.push(filterAction);
    array.push(clearFilterAction);
    return array;
  }

  generateActions: function() {
    const array = [];
    if (withAdd)
      array.push(addAction);
    if (withFilters)
      prepareFiltersAction(array);
    if (withRemove)
      array.push(removeAction);
    if (withPrint)
      array.push(printAction);
    prepareExtraActions(array); // from PaginatedActions
    if (withPagination)
      preparePaginationActions(array);
    return array;
  }

  onWithFiltersChanged:    prepareActions()
  onWithRemoveChanged:     prepareActions()
  onWithPaginationChanged: prepareActions()
  onWithPrintChanged:      prepareActions()

  signal add()
  signal remove()
  signal focusFilters()
  signal clearFilters()
  signal printRequested()

  Action {
    id: addAction
    text: labelForAction("actions.addResource")
    icon.name: "list-add"
    enabled: withAdd
    sequence: "F5"
    onTriggered: add()
  }

  Action {
    id: filterAction
    text: i18n.t("actions.filter")
    icon.name: "edit-find"
    enabled: withFilters
    sequence: "F2"
    onTriggered: focusFilters()
  }

  Action {
    id: clearFilterAction
    text: i18n.t("actions.clear-filters")
    enabled: withFilters
    icon.name: "edit-find"
    sequence: "F3"
    onTriggered: clearFilters()
  }

  Action {
    id: removeAction
    text: labelForAction("actions.removeResource")
    icon.name: "list-remove"
    enabled: withRemove
    sequence: "F6"
    onTriggered: remove()
  }

  Action {
    id: printAction
    text: i18n.t("actions.print")
    icon.name: "printer"
    enabled: withPrint
    sequence: "F9"
    onTriggered: printRequested()
  }
}
