set(TOTO_SRCS
    client.cpp
    main.cpp
    server.cpp
)

add_definitions(
	${MOOST_DEFINITIONS}
)

include_directories(
	${MOOST_INCLUDE_DIR}
)

add_executable(simple_server
	${TOTO_SRCS}
)

target_link_libraries(simple_server
    ${MOOST_STASH_LINK_DEPENDENCIES}
)
