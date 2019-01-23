set(srcs
  src/query_evaluator/query_evaluator.cpp
  )
set(headers
  src/query_evaluator/query_evaluator.h
  )
add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


