#include <cstdio>
// 因为 lua 是 c 写的，且源码中没有加编译选项判断
// 因此要加上 extern "C"
extern "C" {
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

/*
程序目的：
1. 开启 lua 虚拟机
*/

int main() {
  // lua_State 可以理解为一个 lua 代码的运行环境(虚拟机)
  // 1. 创建 lua 虚拟机
  lua_State* state = luaL_newstate();
  // 2. 下载基本库
  luaL_openlibs(state); {
    // some code
    printf("lua run ok!");
  }
  // 3. 回收 lua 虚拟机
  lua_close(state);
  return 0;
}