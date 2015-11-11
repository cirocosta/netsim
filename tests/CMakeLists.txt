
include_directories(${netsim_INCLUDE_DIRS})

file(GLOB_RECURSE tests "*.c")
foreach(file ${tests})

  get_filename_component(testname ${file} NAME_WE)
  add_executable(${testname} ${file})
  target_link_libraries(${testname} ${netsim_LIBRARIES})

  add_test(NAME ${testname} 
           COMMAND ${testname}) 
  add_test(NAME "memcheck_${testname}"
           COMMAND valgrind --leak-check=yes --error-exitcode=1 ./${testname})

endforeach()
