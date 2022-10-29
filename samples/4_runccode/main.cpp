#include <cstdio>
#include <cstring>
#include <cmath>

extern "C" {
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

/*
程序目的： 在 lua 中调用 c 提供的代码
*/

const char* script = R"(
  local val = pow_from_c(2, 10)
  print(string.format("pow: %s", val))
  -- 错误： call function chunk failed: [string "c_closure"]:4: bad argument #2 to 'pow_from_c' (number expected, got no value)
  -- local val2 = pow_from_c(1)
)";

int my_pow(lua_State* state) {
  int a = luaL_checkinteger(state, 1);
  int b = luaL_checkinteger(state, 2);
  int rst = (int)pow(a, b);
  lua_pushinteger(state, rst);
  return 1;
}

int main() {
  lua_State* state = luaL_newstate();
  luaL_openlibs(state); // []
  {
    // 1. 注册代码
    lua_getglobal(state, "_G"); // [_G]
    lua_pushstring(state, "pow_from_c"); // [_G, "pow_from_c"]
    lua_pushcclosure(state, my_pow, 0); // [_G, "pow_from_c", func]
    // 相当于： _G=stack.peek(-3);func=stack.pop();pow_from_c=stack.pop();_G[pow_from_c]=func
    lua_rawset(state, -3); // [_G]

    // 2. 加载脚本
    auto rst = luaL_loadbuffer(state, script, strlen(script), "c_closure");
    if(rst != 0) {
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("load script failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }

    // 3. 调用脚本
    if(lua_pcall(state, 0, 0, 0)) {
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("call function chunk failed: %s\n", msg);
        lua_pop(state, 1);
      }
    }
  }
  lua_close(state);
  return 0;
}

