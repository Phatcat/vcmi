/*
 * LuaWrapper.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#pragma once

#include <lua.hpp>

#include "api/Registry.h"

/*
 * Original code is LunaWrapper by nornagon.
 * https://lua-users.org/wiki/LunaWrapper
 * Published under the BSD 2-clause license
 * https://opensource.org/licenses/BSD-2-Clause
 *
 */

namespace scripting
{

namespace detail
{
	template<typename T>
	struct RegType
	{
		const char * name;
		int(* functor)(lua_State *, T);
	};
}

template<class T, class U = T> class OpaqueWrapper
{
public:
	using Object = T *;
	using RegType = detail::RegType<Object>;

	static int registrator(lua_State * L, api::TypeRegistry * typeRegistry)
	{
		if(luaL_newmetatable(L, typeRegistry->getKey<Object *>()) != 0)
		{
			lua_pushstring(L, "__index");

			lua_newtable(L);

			lua_Integer index = 0;

			for(auto & reg : U::REGISTER)
			{
				lua_pushstring(L, reg.name);
				lua_pushnumber(L, index);
				lua_pushcclosure(L, &OpaqueWrapper<T, U>::dispatcher, 1);
				lua_settable(L, -3);
				index++;
			}

			lua_settable(L, -3);
		}

		return 1;
	}

	static int dispatcher(lua_State * L)
	{
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));

		void * objPtr = luaL_checkudata(L, 1, api::TypeRegistry::get()->getKey<Object *>());

		lua_remove(L, 1);

		if(objPtr)
		{
			auto obj = static_cast<Object *>(objPtr);
			return (U::REGISTER[i].functor)(L, *obj);
		}

		return 0;
	}
};

template<class T, class U = T> class SharedWrapper
{
public:
	using Object = std::shared_ptr<T>;
	using RegType = detail::RegType<Object>;

	static int registrator(lua_State * L, api::TypeRegistry * typeRegistry)
	{
		if(luaL_newmetatable(L, typeRegistry->getKey<Object *>()) != 0)
		{
			lua_Integer index = 0;

			for(auto & reg : U::REGISTER)
			{
				lua_pushstring(L, reg.name);
				lua_pushnumber(L, index);
				lua_pushcclosure(L, &SharedWrapper<T, U>::dispatcher, 1);
				lua_settable(L, -3);
				index++;
			}

			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, &(SharedWrapper<T, U>::destructor));
			lua_settable(L, -3);

			lua_pushstring(L, "__index");
			lua_pushvalue(L, -2);
			lua_settable(L, -3);
		}

		lua_settop(L, 0);

		lua_newtable(L);
		lua_pushstring(L, "new");
		lua_pushcfunction(L, &(SharedWrapper<T, U>::constructor));
		lua_settable(L, -3);

		return 1;
	}

	static int constructor(lua_State * L)
	{
		lua_settop(L, 0);//we do not accept any parameters in constructor

		auto obj = std::make_shared<T>();

		lua_newtable(L);

		void * raw = lua_newuserdata(L, sizeof(Object));

		if(!raw)
		{
			lua_settop(L, 0);
			return 0;
		}

		new(raw) Object(obj);

		luaL_getmetatable(L, api::TypeRegistry::get()->getKey<Object *>());

		if(!lua_istable(L, -1))
		{
			lua_settop(L, 0);
			return 0;
		}

		lua_setmetatable(L, -2);

		return 1;
	}

	static int dispatcher(lua_State * L)
	{
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));

		void * raw = luaL_checkudata(L, 1, api::TypeRegistry::get()->getKey<Object *>());

		if(!raw)
		{
			lua_settop(L, 0);
			return 0;
		}

		auto obj = *(static_cast<Object *>(raw));

		lua_remove(L, 1);

		return (U::REGISTER[i].functor)(L, obj);
	}

	static int destructor(lua_State * L)
	{
		void * objPtr = luaL_checkudata(L, 1, api::TypeRegistry::get()->getKey<Object *>());
		if(objPtr)
		{
			auto obj = static_cast<Object *>(objPtr);
			obj->reset();
		}

		return 0;
	}
};

}
