import QtQuick 2.12

ActionSet {
  property bool withPagination: false
  property int page:    withPagination ? root.controller.page : 0
  property int maxPage: withPagination ? root.controller.maxPage : -1

  function preparePaginationActions(array) {
    array.push(previousPageAction);
    array.push(nextPageAction);
    return array;
  }

  function prepareExtraActions(array) {
  }

  generateActions: function() {
    const array = [];
    prepareExtraActions(array);
    if (withPagination)
      preparePaginationActions(array);
    return array;
  }

  onPreviousPage: root.controller.previousPage()
  onNextPage:     root.controller.nextPage()

  signal previousPage()
  signal nextPage()

  Action {
    id: previousPageAction
    text: i18n.t("actions.previousPage")
    icon.name: "arrow-left"
    enabled: withPagination && page > 0
    sequence: "Alt+Left"
    autoRepeat: true
    onTriggered: previousPage()
  }

  Action {
    id: nextPageAction
    text: i18n.t("actions.nextPage")
    icon.name: "arrow-right"
    enabled: withPagination && (page < maxPage || maxPage === -1)
    sequence: "Alt+Right"
    autoRepeat: true
    onTriggered: nextPage()
  }
}
