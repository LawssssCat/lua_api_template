add_executable(call_return)

target_sources(call_return
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(call_return
PRIVATE
  lua_lib
)