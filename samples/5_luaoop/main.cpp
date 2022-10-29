#include <cstdio>
#include <cstdint>
#include <cstring>
#include <new>

extern "C" {
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

/*
程序目的： lua 调用 c 方法创建对象，并调用对象方法
*/

/*
userdata: {
  metatable: {
    __index = {
      equal = function() {},
      id = function() {}
    }
  }
}
*/

class GameObject {
private:
  uint32_t _id;
public:
  static size_t registry_value;
public:
  GameObject(uint32_t id)
    : _id(id) 
  {}
  const uint32_t id() {
    return _id;
  }
  bool equal(GameObject* obj) {
    return _id == obj->_id;
  }
};

size_t GameObject::registry_value = 0;

const char* script = R"(
  local obj_1 = create_game_object(1)
  local obj_2 = create_game_object(2)
  local obj_3 = create_game_object(3)
  print(string.format("obj_1 equal obj_1: %s", obj_1:equal(obj_1)))
  print(string.format("obj_1 equal obj_2: %s", obj_1:equal(obj_2)))
  print(string.format("obj_1 equal obj_3: %s", obj_1:equal(obj_3)))
  print(string.format("obj_1:id()=%s, obj_2:id()=%s, obj_3:id()=%s", 
    obj_1:id(),
    obj_2:id(),
    obj_3:id()))
)";

int game_object_is_equal(lua_State* state)
{
  if(!lua_isuserdata(state, 1) || !lua_isuserdata(state, 2)) {
    luaL_error(state, "error args number or type");
  }
  GameObject* obj1 = (GameObject*)lua_touserdata(state, 1);
  GameObject* obj2 = (GameObject*)lua_touserdata(state, 2);
  auto rst = obj1->equal(obj2);
  lua_pushboolean(state, rst);
  return 1;
}

int game_object_get_id(lua_State* state)
{
  if(!lua_isuserdata(state, 1)) {
    luaL_error(state, "error args number or type");
  }
  GameObject* obj = (GameObject*)lua_touserdata(state, 1);
  auto rst = obj->id();
  lua_pushinteger(state, rst);
  return 1;
}

int create_game_object(lua_State* state)
{
  auto id = luaL_checkinteger(state, 1);
  void* p = lua_newuserdata(state, sizeof(GameObject));
  // placement new
  GameObject* obj = new(p) GameObject(id); // userdata
  lua_rawgetp(state, LUA_REGISTRYINDEX, &GameObject::registry_value);
  // 相当于： userdata=stack.peek(-2);metatable=stack.pop();setmetatable(userdata, metatable);
  lua_setmetatable(state, -2); 
  return 1;
}

int main() {
  lua_State* state = luaL_newstate();
  luaL_openlibs(state);
  {
    // 注册构造方法
    lua_getglobal(state, "_G"); // [_G]
    lua_pushstring(state, "create_game_object"); // [_G, "create_game_obj"]
    lua_pushcclosure(state, create_game_object, 0); // [_G, "create_game_obj", func]
    lua_rawset(state, -3); // [_G]
    lua_pop(state, 1); // []

    // 注册对象方法
    lua_newtable(state); // [metatable]
    lua_pushstring(state, "__index");
    lua_newtable(state);
    // equal
    lua_pushstring(state, "equal");
    lua_pushcclosure(state, game_object_is_equal, 0);
    lua_rawset(state, -3);
    // id
    lua_pushstring(state, "id");
    lua_pushcclosure(state, game_object_get_id, 0);
    lua_rawset(state, -3);
    lua_rawset(state, -3); // [metateble]
    /*
    在注册表 LUA_REGISTRYINDEX 中，注册 key 为 &GameObject::registry_value， key 为 metatable value 的记录。
    这条记录可以通过 lua_rawgetp 方法，并传入 LUA_REGISTRYINDEX 和 &GameObject::registry_value 重新获取。
    从而在其他地方重新获取到 metatable。
    如： 上面注册的 create_game_obj 方法中，就有使用。
    */
    lua_rawsetp(state, LUA_REGISTRYINDEX, &GameObject::registry_value); // []
  }
  {
    // 加载脚本
    auto rst = luaL_loadbuffer(state, script, strlen(script), "lua_oop");
    if(rst != 0) {
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("load script failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
    // 执行脚本
    if(lua_pcall(state, 0, 0, 0)) {
      if(lua_isstring(state, -1)) {
        auto msg = lua_tostring(state, -1);
        printf("call function chunk failed: %s\n", msg);
        lua_pop(state, 1);
      }
      return -1;
    }
  }
  lua_close(state);
  return 0;
} 