/*
 * LuaCallWrapper.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#pragma once

#include "LuaStack.h"

namespace scripting
{

//TODO:

template <typename T>
class LuaCallWrapper
{
public:
	using Wrapped = typename std::remove_const<T>::type;

	template <typename R, typename U>
	static int wrap(lua_State * L, U * object, R (Wrapped::* method)() const)
	{
		LuaStack S(L);
		auto functor = std::mem_fn(method);
		S.push(functor(object));
		return 1;
	}

	template <typename R, typename U>
	static int wrap(lua_State * L, U * object, R (Wrapped::* method)())
	{
		LuaStack S(L);
		auto functor = std::mem_fn(method);
		S.push(functor(object));
		return 1;
	}
};

}
