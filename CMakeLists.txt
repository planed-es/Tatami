cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(Tatami)

option(BUILD_TESTS "builds tests" ON)

find_package(PkgConfig)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Qml QuickControls2 Network WebSockets)
pkg_check_modules(QI18N REQUIRED Qt${QT_VERSION_MAJOR}I18n>=1.0.0)
pkg_check_modules(METARECORDQT REQUIRED meta-record-qt${QT_VERSION_MAJOR}>=1.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0071 NEW)

include_directories(tatami)

file(GLOB qml_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  imports/com/planed/tatami/*.qml
  imports/com/planed/tatami/*.js
  imports/com/planed/tatami/*.mjs
)
file(GLOB resource_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} fonts/*.ttf)
file(GLOB_RECURSE cpp_headers tatami/*.h)
file(GLOB_RECURSE cpp_sources tatami/*.cpp tatami_plugin.cpp)
file(GLOB tst_headers tatami/tests/*.h)
file(GLOB tst_sources tatami/tests/*.cpp)
list(FILTER cpp_headers EXCLUDE REGEX "tatami/tests/.*")
list(FILTER cpp_sources EXCLUDE REGEX "tatami/tests/.*")

link_libraries(
  Qt${QT_MAJOR_VERSION}::Gui
  Qt${QT_MAJOR_VERSION}::WebSockets
  ${QI18N_LIBRARIES}
  ${METARECORDQT_LIBRARIES}
)

qt_add_qml_module(Tatami
  OUTPUT_DIRECTORY imports/com/planed/tatami
  URI com.planed.tatami
  VERSION 1.0
  QML_FILES ${qml_files}
  SOURCES tatami_plugin.h ${cpp_sources}
  RESOURCES ${resource_files}
)

target_compile_definitions(Tatami PRIVATE TATAMI_LIBRARY)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()

install(FILES tatami_plugin.h DESTINATION include)
install(FILES ${cpp_headers} DESTINATION include/tatami)
install(TARGETS Tatami LIBRARY DESTINATION lib)
install(TARGETS Tatamiplugin LIBRARY DESTINATION lib/tatami/plugins)
