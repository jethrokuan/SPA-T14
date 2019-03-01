file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

if (PROFILING)
  message("******* Profiling active *********")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -pg")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -pg")
endif()

add_executable(profiling ${srcs})


target_link_libraries(profiling spa)