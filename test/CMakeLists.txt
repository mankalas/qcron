# In order to avoid "#include <../" when including headers.
include_directories(${${PROJECT}_INCLUDE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/test)

# Declaration.
set(TESTS
  qcron_test
  syntax_test
  holiday_test
  )

foreach(test ${TESTS})
  project(${test})
  add_executable(${test} ${test}.cpp)
  qt5_use_modules(${test} ${QT_COMPONENTS})
  target_link_libraries(${test} ${PROJECT} qcron)
  set_target_properties(${test} PROPERTIES
    COMPILE_FLAGS "${CMAKE_CXX_FLAGS}")

  if (WIN32)
    set(EXEC ${EXECUTABLE_OUTPUT_PATH}/${test}.exe)
    add_test(${test} ${EXEC})
    get_property(qt5_lib_location TARGET ${Qt5Core_LIBRARIES} PROPERTY LOCATION)
    get_filename_component(MINGW_PATH ${qt5_lib_location} DIRECTORY)
    INSTALL(TARGETS ${test}
      ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR}
      LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR}
      BUNDLE DESTINATION .
      RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}
      COMPONENT runtime)
    set(LIBRARY_DIRS ${MINGW_PATH} ${LIBRARY_OUTPUT_DIR})
    INSTALL(CODE "
	    include(BundleUtilities)
            fixup_bundle(\"${EXEC}\" \"\" \"${LIBRARY_DIRS}\")
	    ")
  else()
    add_test(${test} ${test})
  endif()

endforeach()
