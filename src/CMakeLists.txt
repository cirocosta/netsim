include_directories(${INCLUDE_DIRS} ${READLINE_INCLUDE_DIRS})

add_executable(netsim netsim.c)

file(GLOB_RECURSE srcs "*.c")
list(REMOVE_ITEM srcs 
     "${CMAKE_CURRENT_SOURCE_DIR}/netsim.c")

add_library(libnetsim ${srcs})
target_link_libraries(netsim ${READLINE_LIBRARIES} "libnetsim" "m")

set(LIBRARIES ${LIBRARIES} ${READLINE_LIBRARIES} "libnetsim" "m" PARENT_SCOPE)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${READLINE_INCLUDE_DIRS} PARENT_SCOPE)
