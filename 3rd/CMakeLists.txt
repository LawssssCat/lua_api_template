project(3rd)

set(LUA_LIB_SOURCES
  ${TEMPLATE_ROOT}/3rd/lua/lapi.c
  ${TEMPLATE_ROOT}/3rd/lua/lauxlib.c
  ${TEMPLATE_ROOT}/3rd/lua/lbaselib.c
  ${TEMPLATE_ROOT}/3rd/lua/lcode.c
  ${TEMPLATE_ROOT}/3rd/lua/lcorolib.c
  ${TEMPLATE_ROOT}/3rd/lua/lctype.c
  ${TEMPLATE_ROOT}/3rd/lua/ldblib.c
  ${TEMPLATE_ROOT}/3rd/lua/ldebug.c
  ${TEMPLATE_ROOT}/3rd/lua/ldo.c
  ${TEMPLATE_ROOT}/3rd/lua/ldump.c
  ${TEMPLATE_ROOT}/3rd/lua/lfunc.c
  ${TEMPLATE_ROOT}/3rd/lua/lgc.c
  ${TEMPLATE_ROOT}/3rd/lua/linit.c
  ${TEMPLATE_ROOT}/3rd/lua/liolib.c
  ${TEMPLATE_ROOT}/3rd/lua/llex.c
  ${TEMPLATE_ROOT}/3rd/lua/lmathlib.c
  ${TEMPLATE_ROOT}/3rd/lua/lmem.c
  ${TEMPLATE_ROOT}/3rd/lua/loadlib.c
  ${TEMPLATE_ROOT}/3rd/lua/lobject.c
  ${TEMPLATE_ROOT}/3rd/lua/lopcodes.c
  ${TEMPLATE_ROOT}/3rd/lua/loslib.c
  ${TEMPLATE_ROOT}/3rd/lua/lparser.c
  ${TEMPLATE_ROOT}/3rd/lua/lstate.c
  ${TEMPLATE_ROOT}/3rd/lua/lstring.c
  ${TEMPLATE_ROOT}/3rd/lua/lstrlib.c
  ${TEMPLATE_ROOT}/3rd/lua/ltable.c
  ${TEMPLATE_ROOT}/3rd/lua/ltablib.c
  ${TEMPLATE_ROOT}/3rd/lua/ltablib.c
  ${TEMPLATE_ROOT}/3rd/lua/lua.c
  ${TEMPLATE_ROOT}/3rd/lua/ltm.c
  ${TEMPLATE_ROOT}/3rd/lua/lundump.c
  ${TEMPLATE_ROOT}/3rd/lua/lutf8lib.c
  ${TEMPLATE_ROOT}/3rd/lua/lvm.c
  ${TEMPLATE_ROOT}/3rd/lua/lzio.c
)

add_executable(lua ${LUA_LIB_SOURCES})
add_library(lua_lib STATIC)
target_sources(lua_lib
PRIVATE 
  ${LUA_LIB_SOURCES}
)
# 重命名
SET_TARGET_PROPERTIES(lua_lib PROPERTIES OUTPUT_NAME "lua")
SET_TARGET_PROPERTIES(lua_lib PROPERTIES CLEAN_DIRECT_OUTPUT 1)


target_include_directories(lua_lib
INTERFACE
    ${TEMPLATE_ROOT}/3rd
)