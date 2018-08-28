/*
 * CalculateActualDamageImpl.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#pragma once

#include <vcmi/events/CalculateActualDamage.h>

namespace events
{

class CalculateActualDamageImpl : public CalculateActualDamage
{
public:
	void execute(const EventBus * bus) override;
};

}
