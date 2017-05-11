set(adaptorLIBS
        ${Boost_LIBRARIES}
        NEPOMUKadaptor
        NEPOMUKproto
        NEPOMUKtool)

set(adaptorINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(adaptor_dictionary dictionary.cc "${adaptorLIBS}" "${adaptorINCLUDES}")
