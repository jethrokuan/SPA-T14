set(srcs
  src/simple_parser/lexer.cpp
  )

set(headers
  src/simple_parser/lexer.h
  src/simple_parser/token.h
  src/simple_parser/parser.h
  )

add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
