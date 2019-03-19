# Simple Component
set(srcs ${srcs}
  src/main.cpp
  )


file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(viz ${srcs})

target_link_libraries(viz spa)
