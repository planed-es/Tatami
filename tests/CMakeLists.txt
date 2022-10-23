set(TatamiLinkLibraries
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Qml
)

add_library(TatamiTest SHARED ${cpp_sources} ${tst_sources})
target_compile_definitions(TatamiTest PRIVATE TATAMI_LIBRARY TATAMI_TESTLIB)
target_link_libraries(TatamiTest ${TatamiLinkLibraries})

install(FILES ${tst_headers} DESTINATION include/tatami/tests)
install(TARGETS TatamiTest LIBRARY DESTINATION lib)
