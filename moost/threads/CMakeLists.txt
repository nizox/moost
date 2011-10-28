if (DEFINED UNIX)
    set(THREAD_SOURCES
        posix/posix_thread.cpp
        posix/posix_cond.cpp
        posix/posix_mutex.cpp
        posix/posix_shared.cpp
        )
    set(THREAD_LINK_DEPENDENCIES
        pthread
        )
elseif (DEFINED WIN32)
    set(THREAD_SOURCES
        win/win_thread.cpp
        win/win_cond.cpp
        win/win_mutex.cpp
	win/win_shared.cpp
        )
endif()

include_directories(
    ${MOOST_INCLUDE_DIR}
)

add_library(
    moost_thread
    ${THREAD_SOURCES}
)

target_link_libraries(moost_thread
    ${THREAD_LINK_DEPENDENCIES}
)

set(THREAD_LINK_DEPENDENCIES
    moost_thread
    ${THREAD_LINK_DEPENDENCIES}
PARENT_SCOPE)
