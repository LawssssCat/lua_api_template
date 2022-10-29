#include <cstdio>
#include <cstring>

extern "C" {
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

const char* script = R"(
  test.hello();
)";

static int test_hello(lua_State* L) {
  printf("hello, lua!\n");
  return 0;
}

static const luaL_Reg testlib[] = {
  {"hello", test_hello},
  {NULL, NULL}
};

int luaopen_test(lua_State* L)
{
  luaL_newlib(L, testlib);
  return 1;
}

int main()
{
  lua_State* state = luaL_newstate();
  luaL_openlibs(state);
  {
    // 加载
    luaL_requiref(state, "test", luaopen_test, 1);
    lua_pop(state, 1);
    // 加载
    auto rst = luaL_loadbuffer(state, script, strlen(script), 0);
    if(rst != 0) {
      if(lua_isstring(state, -1)) 
      {
        auto msg = lua_tostring(state, -1);
        printf("load script failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
    // 执行
    if(lua_pcall(state, 0, 0, 0)) {
      if(lua_isstring(state, -1)) 
      {
        auto msg = lua_tostring(state, -1);
        printf("load script failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
  }
  lua_close(state);
  printf("lua run ok!\n");
  return 0;
}