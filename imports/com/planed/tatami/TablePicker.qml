import QtQuick 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
  id: root
  property QtObject controller
  property var filterProperties
  property QtObject filterModel
  property alias currentIndex: table.currentIndex
  property alias hasFocus: filters.hasFocus
  property alias mainColumn: table.mainColumn

  signal picked(QtObject model)

  function forceActiveFocus() {
    filters.takeFocus();
  }

  function clearFilters() {
    filters.clear();
  }

  Filter {
    id: filters
    Layout.preferredHeight: 80
    Layout.fillWidth: true
    model: filterModel
    propertyList: root.filterProperties
    onFilterUpdate: controller.filters = filterData
  }

  TableView {
    id: table
    Layout.fillHeight: true
    Layout.fillWidth: true
    model: controller.table
    navigationEnabled: filters.hasFocus
    onActivated: picked(model) // deprecated, but model is null otherwise
  }
}
