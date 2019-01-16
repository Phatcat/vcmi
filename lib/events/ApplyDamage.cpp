/*
 * ApplyDamage.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#include "StdInc.h"

#include <vcmi/events/ApplyDamage.h>
#include <vcmi/Environment.h>

#include "../../lib/NetPacks.h"

namespace events
{

ApplyDamage::ApplyDamage(const Environment * env_, BattleStackAttacked * pack_, std::shared_ptr<battle::Unit> target_)
	: env(env_),
	pack(pack_),
	target(target_)

{
	initalDamage = pack->damageAmount;
}

void ApplyDamage::execute()
{
	//TODO: move CStack::prepareAttacked code here, except REBIRTH
}

SubscriptionRegistry<ApplyDamage> * ApplyDamage::getRegistry()
{
	static std::unique_ptr<SubscriptionRegistry<ApplyDamage>> Instance = make_unique<SubscriptionRegistry<ApplyDamage>>();
	return Instance.get();
}

int64_t ApplyDamage::getInitalDamage()
{
	return initalDamage;
}

int64_t ApplyDamage::getDamage()
{
	return pack->damageAmount;
}

void ApplyDamage::setDamage(int64_t value)
{
	pack->damageAmount = value;
}


};
