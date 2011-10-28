set(TOTO_SRCS
    main.cpp
)

add_definitions(
	${MOOST_DEFINITIONS}
)

include_directories(
	${MOOST_INCLUDE_DIR}
)

add_executable(timer
	${TOTO_SRCS}
)

target_link_libraries(timer
    ${MOOST_STASH_LINK_DEPENDENCIES}
)
