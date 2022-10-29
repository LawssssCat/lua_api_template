add_executable(run_c_code)

target_sources(run_c_code
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(run_c_code
PRIVATE
  lua_lib
)