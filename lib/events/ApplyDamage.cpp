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

#include "../../lib/NetPacks.h"

namespace events
{

ApplyDamage::ApplyDamage(BattleStackAttacked * pack_, std::shared_ptr<battle::Unit> target_)
	: pack(pack_),
	target(target_)
{

}

void ApplyDamage::execute(const EventBus * bus)
{

}

SubscriptionRegistry<ApplyDamage> * ApplyDamage::getRegistry()
{
	static std::unique_ptr<SubscriptionRegistry<ApplyDamage>> Instance = make_unique<SubscriptionRegistry<ApplyDamage>>();
	return Instance.get();
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
