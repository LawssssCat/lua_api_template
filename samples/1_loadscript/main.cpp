#include <cstdio>
#include <cstring>

extern "C" {
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

/*
程序目的：
1. 加载 lua 脚本
2. 运行 lua 脚本
*/

// lua 脚本
const char* script = R"(
-- 加载时错误： load script failed: [string "hello"]:4: syntax error near 'function'
-- aaaaaaaaaaaaaaaaaaaaaa
function hello()
  print("hello, lua!")
  -- 运行时错误： call function chunk failed: [string "hello"]:7: attempt to call a nil value (global 'bbbbbbbbbbbbbbbbb')
  -- bbbbbbbbbbbbbbbbb()
end
hello()
)";

int main() {
  lua_State* state = luaL_newstate();
  luaL_openlibs(state); // 栈情况： []
  {
    printf("lua init ok!\n");
    auto rst = luaL_loadbuffer(state, script, strlen(script), "hello"); // 加载脚本 chunk。 栈情况： [chunk]
    if(rst != 0) { // 加载失败
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1); // 栈顶
        printf("load script failed: %s\n", msg);
        lua_pop(state, 1); // 栈顶错误信息弹出
      }
      return -1;
    }
    printf("lua load ok!\n");
    auto f = lua_pcall(state, 0, 0, 0); // 不设置参数，返回，错误。 栈情况： [] 或者 ["lua error message..."]
    if(f) { // 返回不为 0，说明有错误
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("call function chunk failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
    printf("lua run ok!\n");
  }
  lua_close(state);
  return 0;
}