function(PROTOBUF_GENERATE_CPP HDRS SRCS)
    # input validation
    #if(not ARGN)
    #    message(SEND_ERROR "Error: Need to specify source files to generate protocol buffer sources.")
    #    return()
    #endif()

    #prepare output variables
    set(${HDRS} "")
    set(${SRCS} "")

    # remember include paths for all pb files
    foreach(PROTO ${ARGN})
        get_filename_component(ABSOLUTE_PROTO ${PROTO} ABSOLUTE)
        get_filename_component(ABSOLUTE_PATH_PROTO ${ABSOLUTE_PROTO} PATH)
        list(FIND _proto_include_dirs "${ABSOLUTE_PATH_PROTO}" _contained)
        if(${_contained} EQUAL -1)
            list(APPEND _proto_include_dirs ${ABSOLUTE_PATH_PROTO})
        endif()
    endforeach()


    message(STATUS "Using protoc: ${Mason_PROTOC}")

    #generate .pb.h and .pb.cc for each input .proto
    foreach(PROTO ${ARGN})
        get_filename_component(ABSOLUTE_PROTO ${PROTO} ABSOLUTE)
        get_filename_component(NAME_PROTO ${PROTO} NAME_WE)

        # remember headers/srs for output
        set(HEADER_FILENAME "${CMAKE_CURRENT_BINARY_DIR}/${NAME_PROTO}.pb.h")
        set(SRC_FILENAME "${CMAKE_CURRENT_BINARY_DIR}/${NAME_PROTO}.pb.cc")

        list(APPEND ${HDRS} ${HEADER_FILENAME})
        list(APPEND ${SRCS} ${SRC_FILENAME})

        # add so commands for generation
        add_custom_command(
          OUTPUT ${HEADER_FILENAME}
                 ${SRC_FILENAME}
          COMMAND ${Mason_PROTOC}
          ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR} -I ${_proto_include_dirs} ${ABSOLUTE_PROTO}
          DEPENDS ${ABSOLUTE_PROTO}
          COMMENT "Compiling ${ABSOLUTE_PROTO} using the protoc c++ compiler."
          VERBATIM)
    endforeach()

    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()
