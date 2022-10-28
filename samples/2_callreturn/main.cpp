#include <cstdio>
#include <cstring>

extern "C" {
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

/*
程序目的：
1. 调用函数、传递参数、接收返回值
*/

// lua 脚本
const char* script = R"(
function my_pow(x,y)
  return x^y
end
)";

int main() {
  lua_State* state = luaL_newstate();
  luaL_openlibs(state); {
    printf("lua init ok!\n");
    auto rst = luaL_loadbuffer(state, script, strlen(script), "hello"); // 加载脚本
    if(rst != 0) { // 加载失败
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1); // 栈顶
        printf("load script failed: %s\n", msg);
        lua_pop(state, 1); // 栈顶错误信息弹出
      }
      return -1;
    }
    printf("lua load ok!\n");
    // 运行脚本
    rst = lua_pcall(state, 0, 0, 0); // 不设置参数，返回，错误
    if(rst) { // 脚本运行错误
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("call function chunk failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1; 
    }
    printf("lua run ok!\n");
    // 调用脚本函数
    auto func_mane = "my_pow";
    // 1. 全局查找变量，并将其压入栈顶
    rst = lua_getglobal(state, func_mane);
    if(!lua_isfunction(state, -1)) { // 判断栈顶是否函数
      printf("function name \"%s\" not found!\n", func_mane);
      return -1;
    }
    // 2. 参数压入栈
    lua_pushnumber(state, 2);
    lua_pushnumber(state, 10);
    // 3. 调用函数
    rst = lua_pcall(state, 2, 1, 0);
    if(rst != 0) {
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("call function chunk failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
    lua_Number val = lua_tonumber(state, -1);
    printf("%s: %f\n", func_mane, val);
    lua_pop(state, 1);
  }
  lua_close(state);
  return 0;
}