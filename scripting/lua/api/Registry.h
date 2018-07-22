/*
 * Registry.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#pragma once

#include <lua.hpp>

#define VCMI_REGISTER_SCRIPT_API(Type, Name) \
namespace\
{\
RegisterAPI<Type> _register(Name);\
}\
\

namespace scripting
{
namespace api
{
class TypeRegistry;

class Registar
{
public:
	virtual ~Registar() = default;

	virtual void perform(lua_State * L, TypeRegistry * typeRegistry) const = 0;
};

class Registry : public boost::noncopyable
{
public:
	static Registry * get();

	const Registar * find(const std::string & name) const;
	void add(const std::string & name, std::shared_ptr<Registar> item);
private:
	std::map<std::string, std::shared_ptr<Registar>> data;

	Registry();
};

template<typename T>
class RegistarT : public Registar
{
public:
	void perform(lua_State * L, TypeRegistry * typeRegistry) const override
	{
		T::registrator(L, typeRegistry);
	}
};

template<typename T>
class RegisterAPI
{
public:
	RegisterAPI(const std::string & name)
	{
		auto r = std::make_shared<RegistarT<T>>();
		Registry::get()->add(name, r);
	}
};

class TypeRegistry : public boost::noncopyable
{
public:
	template<typename T>
	const char * getKey(const T * t = nullptr)
	{
		if(t)
			return getKeyForType(typeid(*t));
		else
			return getKeyForType(typeid(T));
	}

	static TypeRegistry * get();
private:
	size_t nextIndex;

	boost::mutex mutex;//FIXME: remove and make TypeRegistry part of Context

	std::map<std::type_index, std::string> keys;

	TypeRegistry();

	const char * getKeyForType(const std::type_info & type);
};

}
}
