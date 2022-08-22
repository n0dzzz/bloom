#pragma once
#include "../Math/Vector.h"

class CCollideable
{
public:
	Vector& Mins()
	{
		using original_fn = Vector & (__thiscall*)(void*);
		return (*(original_fn**)this)[1](this);
	}
	Vector& Maxs()
	{
		using original_fn = Vector & (__thiscall*)(void*);
		return (*(original_fn**)this)[2](this);
	}
};
