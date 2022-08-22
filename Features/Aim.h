#pragma once
#include "../SDK/SDK.h"
#include "../SDK/Interfaces/CUserCmd.h"
#include "../Utilities/Config/Config.h"
#include "../Game.h"

namespace Aim
{
	inline Vector AimAngle;
	inline float Hitchance;
	inline CBasePlayer* AimTarget;

	void Run(CUserCmd* cmd);	
}
