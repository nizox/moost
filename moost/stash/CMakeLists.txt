if (DEFINED BNET)
	find_package(Boost REQUIRED system filesystem)

	set(STASH_DEFINITIONS
		-DBNET
	)

	set(STASH_INCLUDE_DIR
		${Boost_INCLUDE_DIR}
	)

	set(stash_sources 
		boost/BoostDispatcher.cpp
		boost/BoostTimer.cpp
	)

	set(STASH_LINK_DEPENDENCIES
		${Boost_LIBRARIES}
	)

	add_library(
		moost_stash
		${stash_sources}
	)

	set(STASH_LINK_DEPENDENCIES
			${STASH_LINK_DEPENDENCIES}
			moost_stash
	PARENT_SCOPE)

elseif (DEFINED UNIX)

	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		set(STASH_DEFINITIONS
			-DUNIX -DLINUX
		)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
		set(STASH_DEFINITIONS
			-DUNIX -DFREEBSD
		)
        else()
		set(STASH_DEFINITIONS
			-DUNIX)
	endif()


elseif (DEFINED WIN32)
		set(STASH_DEFINITIONS
			-DWINDOWS
		)

		if (DEFINED WIN32)
			set(STASH_LINK_DEPENDENCIES
				Ws2_32
			)
		endif()
endif()

add_definitions(
	${MOOST_DEFINITIONS}
	${STASH_DEFINITIONS}
)

include_directories(
	${MOOST_INCLUDE_DIR}
	${STASH_INCLUDE_DIR}
)

set(STASH_DEFINITIONS
	${STASH_DEFINITIONS}
PARENT_SCOPE)

set(STASH_INCLUDE_DIR
	${STASH_INCLUDE_DIR}
PARENT_SCOPE)

