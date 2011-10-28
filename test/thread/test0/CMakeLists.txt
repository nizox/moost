set(TOTO_SRCS
    main.cpp
)

add_definitions(
	${MOOST_DEFINITIONS}
)

include_directories(
	${MOOST_INCLUDE_DIR}
)

add_executable(test0
	${TOTO_SRCS}
)

target_link_libraries(test0
	${MOOST_THREAD_LINK_DEPENDENCIES}
)
