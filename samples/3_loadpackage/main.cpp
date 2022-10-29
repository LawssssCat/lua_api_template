#include <cstdio>
#include <cstring>
#include <cassert>

extern "C" {
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

/*
程序目的： 实现包的功能
  1. 加载包脚本 1
  2. 设定包环境（通过 upvalue 实现），同时添加全局引用 pkg 指向该包环境
    _ENV = {
      metatable = {
        __index = _G
      }
    }
    _G["pkg"] = _ENV
  3. 运行包脚本，得到包内定义的函数
    _ENV = {
      metatable = {
        __index = _G
      }
      hello = function ...
    }
  4. 加载包脚本 2
  5. 运行新的包脚本（脚本内部执行前面包装好的函数）
    pkg.hello()
*/

const char* script_1 = R"(
  function hello(name)
    print('<========== hello, lua! from '..name)
  end
  hello("script_1")
)";

const char* script_2 = R"(
  pkg.hello("script_2")
  pkg.print("<---------- pkg print")
)";

int main() {
  lua_State *state = luaL_newstate();
  luaL_openlibs(state); // 相当于： stack=Stack.new(); 栈情况： []
  {
    printf("lua init ok!\n");

    // 1. 加载包脚本 1
    {
      auto rst = luaL_loadbuffer(state, script_1, strlen(script_1), "script_1"); // 栈情况： [chunk]
      if(rst != 0) {
        if(lua_isstring(state, -1)) {
          auto msg = lua_tostring(state, -1);
          printf("load script failed: %s\n", msg);
          lua_pop(state, 1);
        } else {
          printf("unkonwn error!\n");
        }
        return -1;
      }
    }
    printf("lua load script_1 ok!\n");

    {
      // 【这一步可省略】
      // 验证 upvalue 第一个变量的名字、类型
      /*
      lua_load 加载 lua 代码后会返回一个函数，默认会给这个函数设置一个 upvalue 就叫 _ENV，起值是 LUA_RIDX_GLOBALS 的全局变量表，
      你可以 lua_getglobal 获取这个函数名叫 _ENV 的 upvalue （即下标 1 的 upvalue），因为这个位置是这个函数的 _ENV 表存放位置
      */
      const char* g1 = lua_getupvalue(state, -1, 1); // 栈情况： [chunk, chunk_upvalue_1]
      printf("chunk upvalue key 1: %s\n", g1);
      if(lua_istable(state, -1)) {
        printf("type of chunk upvalue val 1 is a table\n");
      } else {
        printf("type of chunk upvalue val 1 is not a table\n");
      }
      lua_pop(state, 1); // 栈情况： [chunk]
    }

    // 2. 设定包环境（通过 upvalue 实现）
    {
      lua_getglobal(state, "_G"); // 栈情况： [chunk, _G]
      assert(lua_istable(state, -1) && "fail to get _G variable!");
      lua_newtable(state); // 栈情况： [chunk, _G, new_table]
      lua_newtable(state); // 栈情况： [chunk, _G, new_table, metatable]
      lua_pushstring(state, "__index"); // 栈情况： [chunk, _G, new_table, metatable, "__index"]
      lua_pushvalue(state, -4); // 栈情况： [chunk, _G, new_table, metatable, "__index", _G]
      // 相当于： metatable=stack.peek(-3);_G=stack.pop();__index=stack.pop();metatable[__index]=_G;
      lua_rawset(state, -3); // 栈情况： [chunk, _G, new_table, metatable]
      // 相当于： new_table=stack.peek(-2);metatable=stack.pop();setmetatable(new_table, metatable);
      lua_setmetatable(state, -2); // 栈情况： [chunk, _G, new_table]
      lua_pushstring(state, "pkg"); // 栈情况： [chunk, _G, new_table, "pkg"]
      lua_pushvalue(state, -2); // 栈情况： [chunk, _G, new_table, "pkg", new_table]
      // 相当于： _G=stack.peek(-4);pkg=stack.pop();new_table=stack.pop();_G[pkg]=new_table
      lua_rawset(state, -4); // 栈情况： [chunk, _G, new_table]
      // 相当于： chunk=stack.peek(-3);new_table=stack.pop();debug.setupvalue(f, 1, v);
      const char* upvalue_name = lua_setupvalue(state, -3, 1); // 栈情况： [chunk, _G]
      assert(strcmp(upvalue_name, "_ENV")==0 && "upvalue name must be '_ENV'");
      lua_pop(state, 1); // 栈情况： [chunk]
    }
    printf("lua set upvalue ok!\n");

    // 3. 运行包脚本，得到包内定义的函数
    if(lua_pcall(state, 0, 0, 0)) { // []
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("call function chunk failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
    printf("lua run script_1 ok!\n");

    // 4. 加载包脚本 2
    {
      auto rst = luaL_loadbuffer(state, script_2, strlen(script_2), "script_2"); // [chunk]
      if(rst != 0) {
        if(lua_isstring(state, -1)) {
          auto msg = lua_tostring(state, -1);
          printf("load script failed: %s\n", msg);
          lua_pop(state, 1);
        } else {
          printf("unkonwn error!\n");
        }
        return -1;
      }
    }
    printf("lua load script_2 ok!\n");

    // 5. 运行新的包脚本（脚本内部执行前面包装好的函数）
    if(lua_pcall(state, 0, 0, 0)) { // []
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("call function chunk failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
    printf("lua run script_2 ok!\n");

  }
  lua_close(state);
  return 0;
}