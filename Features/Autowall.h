#pragma once
#include "../SDK/SDK.h"
#include "../SDK/Interfaces/CUserCmd.h"
#include "../Utilities/Config/Config.h"
#include "../Game.h"

struct FireBulletData_t
{
	Vector			vecPosition = { };
	Vector			vecDirection = { };
	trace_t			enterTrace = { };
	float			flCurrentDamage = 0.0f;
	int				iPenetrateCount = 0;
};

namespace Autowall
{

}