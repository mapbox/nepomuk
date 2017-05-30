set(ipcLIBS
        ${Boost_LIBRARIES}
        ${MAYBE_PHTREAD_LIBRARIES}
		NEPOMUKadaptor
	    NEPOMUKannotate
	    NEPOMUKtool
	    NEPOMUKipc
		NEPOMUKipcproto
	    NEPOMUKdate
	    NEPOMUKgeometric
	    NEPOMUKnavigation
	    NEPOMUKsearch
	    NEPOMUKservice
	    NEPOMUKtimetable)

set(ipcINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(ipc_route route.cc "${ipcLIBS}" "${ipcINCLUDES}")
add_unit_test(ipc_tile tile.cc "${ipcLIBS}" "${ipcINCLUDES}")
add_unit_test(invalid-requests-responses invalid-requests-responses.cc "${ipcLIBS}" "${ipcINCLUDES}")
add_unit_test(invalid-requests invalid-requests.cc "${ipcLIBS}" "${ipcINCLUDES}")
