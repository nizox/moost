set(loader_SRCS
    loader.cpp
)

set(simple_SRCS
    simple.cpp
)

set(pwet_SRCS
    pwet.cpp
)

add_definitions(
	${MOOST_DEFINITIONS}
)

include_directories(
	${MOOST_INCLUDE_DIR}
)

add_library(simple SHARED
	${simple_SRCS}
)

add_library(pwet SHARED
	${pwet_SRCS}
)

add_executable(loader
	${loader_SRCS}
)

target_link_libraries(loader
    ${MOOST_IC_LINK_DEPENDENCIES}
)
