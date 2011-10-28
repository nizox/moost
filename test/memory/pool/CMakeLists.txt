set(TOTO_SRCS
    main.cpp
)

add_definitions(
	${MOOST_DEFINITIONS}
)

include_directories(
	${MOOST_INCLUDE_DIR}
)

add_executable(pool
	${TOTO_SRCS}
)

target_link_libraries(pool
	${MOOST_MEMORY_LINK_DEPENDENCIES}
)
