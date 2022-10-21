import QtQuick 2.12
import QtQuick.Controls 2.12

ActionSet {
  property bool withAdd: true
  property bool withFilters: false
  property bool withRemove: false
  property bool withPagination: false
  property bool withPrint: false
  property int page:    withPagination ? root.controller.page : 0
  property int maxPage: withPagination ? root.controller.maxPage : -1

  function labelForAction(actionName) {
    return i18n.t(actionName, { resourceName: i18n.t("model." + view.resourceName) });
  }

  function preparePaginationActions(array) {
    array.push(previousPageAction);
    array.push(nextPageAction);
    return array;
  }

  function prepareFiltersAction(array) {
    array.push(filterAction);
    array.push(clearFilterAction);
    return array;
  }

  function prepareExtraActions(array) {
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
    prepareExtraActions(array);
    if (withPagination)
      preparePaginationActions(array);
    return array;
  }

  onWithFiltersChanged:    prepareActions()
  onWithRemoveChanged:     prepareActions()
  onWithPaginationChanged: prepareActions()
  onWithPrintChanged:      prepareActions()
  onPreviousPage:          root.controller.previousPage()
  onNextPage:              root.controller.nextPage()

  signal add()
  signal remove()
  signal focusFilters()
  signal clearFilters()
  signal previousPage()
  signal nextPage()
  signal printRequested()

  Action {
    id: addAction
    text: labelForAction("actions.addResource")
    icon.name: "list-add"
    enabled: withAdd
    shortcut: Shortcut {
      context: Qt.ApplicationShortcut
      sequence: "F5"
      enabled: addAction.enabled
      onActivated: addAction.trigger()
    }
    onTriggered: add()
  }

  Action {
    id: filterAction
    text: i18n.t("actions.filter")
    icon.name: "edit-find"
    enabled: withFilters
    shortcut: Shortcut {
      context: Qt.ApplicationShortcut
      sequence: "F2"
      enabled: withFilters
      onActivated: filterAction.trigger()
    }
    onTriggered: focusFilters()
  }

  Action {
    id: clearFilterAction
    text: i18n.t("actions.clear-filters")
    enabled: withFilters
    icon.name: "edit-find"
    shortcut: Shortcut {
      context: Qt.ApplicationShortcut
      sequence: "F3"
      enabled: withFilters
      onActivated: clearFilterAction.trigger()
    }
    onTriggered: clearFilters()
  }

  Action {
    id: removeAction
    text: labelForAction("actions.removeResource")
    icon.name: "list-remove"
    enabled: withRemove
    shortcut: Shortcut {
      sequence: "F6"
      enabled: withRemove
      onActivated: removeAction.trigger()
    }
    onTriggered: remove()
  }

  Action {
    id: printAction
    text: i18n.t("actions.print")
    icon.name: "printer"
    enabled: withPrint
    shortcut: Shortcut {
      sequence: "F9"
      enabled: withPrint
      onActivated: printAction.trigger()
    }
    onTriggered: printRequested()
  }

  Action {
    id: previousPageAction
    text: i18n.t("actions.previousPage")
    icon.name: "arrow-left"
    enabled: withPagination && page > 0
    shortcut: Shortcut {
      sequence: "Alt+Left"
      enabled: previousPageAction.enabled
      onActivated: previousPageAction.trigger()
    }
    onTriggered: previousPage()
  }

  Action {
    id: nextPageAction
    text: i18n.t("actions.nextPage")
    icon.name: "arrow-right"
    enabled: withPagination && (page < maxPage || maxPage === -1)
    shortcut: Shortcut {
      sequence: "Alt+Right"
      enabled: nextPageAction.enabled
      onActivated: nextPageAction.trigger()
    }
    onTriggered: nextPage()
  }
}
