set(
    memory_sources
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

##add_library(
#    moost_memory
#    ${utils_sources}
#)

set(MEMORY_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
PARENT_SCOPE)

#set(MEMORY_LINK_DEPENDENCIES
#    moost_memory
#PARENT_SCOPE)
