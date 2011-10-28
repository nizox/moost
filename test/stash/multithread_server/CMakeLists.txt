set(TOTO_SRCS
    client.cpp
    main.cpp
    unit.cpp
    unit_pool.cpp
    server.cpp
)

add_definitions(
	${MOOST_DEFINITIONS}
)

include_directories(
	${MOOST_INCLUDE_DIR}
)

add_executable(multithread_server
	${TOTO_SRCS}
)

target_link_libraries(multithread_server
    ${MOOST_STASH_LINK_DEPENDENCIES}
    ${MOOST_THREAD_LINK_DEPENDENCIES}
)
