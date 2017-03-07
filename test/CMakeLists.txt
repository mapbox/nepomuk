macro(add_unit_test target source libs includes)
    add_executable("test_${target}"
        ${source})

    target_link_libraries("test_${target}"
        ${libs}
        ${MAYBE_COVERAGE_LIBRARIES})

    target_include_directories("test_${target}" SYSTEM PUBLIC
          ${includes})

    add_test(NAME "test_${target}"
             COMMAND "test_${target}"
             WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endmacro()

add_subdirectory(tool)
add_subdirectory(gtfs)
add_subdirectory(algorithm)
