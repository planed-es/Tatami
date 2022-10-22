import QtQuick 2.12
import QtQuick.Layouts 1.12
import "." as Tatami

ColumnLayout {
  id: root
  property QtObject controller
  property var filterProperties
  property QtObject filterModel
  property int currentIndex: table.currentIndex
  property bool hasFocus: filters.hasFocus
  property string mainColumn: "name"

  signal picked(QtObject model)

  function forceActiveFocus() {
    filters.takeFocus();
  }

  function clearFilters() {
    filters.clear();
  }

  onCurrentIndexChanged: {
    if (table.currentIndex != currentIndex)
      table.currentIndex = currentIndex;
  }

  Tatami.Filter {
    id: filters
    Layout.preferredHeight: 80
    Layout.fillWidth: true
    model: filterModel
    propertyList: root.filterProperties
    onFilterUpdate: controller.filters = filterData
  }

  Tatami.TableView {
    id: table
    Layout.fillHeight: true
    Layout.fillWidth: true
    model: controller.table
    mainColumn: root.mainColumn
    navigationEnabled: filters.hasFocus
    onActivated: picked(model) // deprecated, but model is null otherwise
  }
}
