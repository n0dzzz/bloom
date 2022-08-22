#pragma once
#include "../Structs/WeaponInfo.h"

class IWeaponSystem
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual CBaseWeaponInfo* GetWeaponData(unsigned int idx) const = 0;
};