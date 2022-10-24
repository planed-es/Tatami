cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(Tatami)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml QuickControls2 Network WebSockets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0071 NEW)

include_directories(/tmp/usr/include tatami)
link_directories(/tmp/usr/lib)

file(GLOB_RECURSE cpp_headers tatami/*.h)
file(GLOB_RECURSE cpp_sources tatami/*.cpp)
file(GLOB tst_headers tatami/tests/*.h)
file(GLOB tst_sources tatami/tests/*.cpp)
list(FILTER cpp_headers EXCLUDE REGEX "tatami/tests/.*")
list(FILTER cpp_sources EXCLUDE REGEX "tatami/tests/.*")

set(qml_files
  imports/com/planed/tatami/View.qml
  imports/com/planed/tatami/Window.qml
  imports/com/planed/tatami/Application.qml
  imports/com/planed/tatami/ActionSet.qml
  imports/com/planed/tatami/Menu.qml
  imports/com/planed/tatami/IndexActions.qml
  imports/com/planed/tatami/EditActions.qml
  imports/com/planed/tatami/MultiplePickerActions.qml
  imports/com/planed/tatami/BooleanField.qml
  imports/com/planed/tatami/DateField.qml
  imports/com/planed/tatami/NumberField.qml
  imports/com/planed/tatami/PickerField.qml
  imports/com/planed/tatami/TextField.qml
  imports/com/planed/tatami/Dialog.qml
  imports/com/planed/tatami/ConfirmDialog.qml
  imports/com/planed/tatami/MessageDialog.qml
  imports/com/planed/tatami/ToolBar.qml
  imports/com/planed/tatami/ToolButton.qml
  imports/com/planed/tatami/Breadcrumbs.qml
  imports/com/planed/tatami/Fields.qml
  imports/com/planed/tatami/Filter.qml
  imports/com/planed/tatami/FormGroup.qml
  imports/com/planed/tatami/Form.qml
  imports/com/planed/tatami/FormLabel.qml
  imports/com/planed/tatami/ShortcutHint.qml
  imports/com/planed/tatami/GridTable.qml
  imports/com/planed/tatami/TableView.qml
  imports/com/planed/tatami/TableEditor.qml
  imports/com/planed/tatami/TablePicker.qml
  imports/com/planed/tatami/TableCell.qml
  imports/com/planed/tatami/TableNavigation.qml
  imports/com/planed/tatami/TableStyle.qml
  imports/com/planed/tatami/TableFields.qml
  imports/com/planed/tatami/TableVerticalHeader.qml
  imports/com/planed/tatami/TableHorizontalHeader.qml
  imports/com/planed/tatami/TableHelpers.js
)

set(resource_files
  fonts/ShareTechMono-Regular.ttf
  fonts/signika.ttf
)

qt_add_qml_module(Tatami
  OUTPUT_DIRECTORY imports/com/planed/tatami
  URI com.planed.tatami
  VERSION 1.0
  QML_FILES ${qml_files}
  SOURCES tatami_plugin.cpp tatami_plugin.h ${cpp_sources}
  RESOURCES ${resource_files}
)

target_compile_definitions(Tatami PRIVATE TATAMI_LIBRARY)

add_subdirectory(tests)

install(FILES tatami_plugin.h DESTINATION include)
install(FILES ${cpp_headers} DESTINATION include/tatami)
install(TARGETS Tatami LIBRARY DESTINATION lib)
install(TARGETS Tatamiplugin LIBRARY DESTINATION lib/tatami/plugins)
