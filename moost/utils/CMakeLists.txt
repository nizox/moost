set(
	utils_sources
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
	moost_utils
	${utils_sources}
)

set(UTILS_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
PARENT_SCOPE)

set(UTILS_LINK_DEPENDENCIES
	moost_utils
PARENT_SCOPE)
