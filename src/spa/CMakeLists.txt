file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/query_evaluator/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/query_evaluator/*.h")
add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


