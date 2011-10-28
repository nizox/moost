

set(FS_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}
PARENT_SCOPE)

if (DEFINED UNIX)
	set(FS_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/unix/FileSystem.cpp
		)
elseif (DEFINED WIN32)
	set(FS_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/win32/FileSystem.cpp
		)
endif()

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	)

add_library(moost_fs
	STATIC
	${FS_SOURCES}
)

set(FS_LINK_DEPENDENCIES 
	moost_fs
PARENT_SCOPE)