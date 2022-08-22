#pragma once
#include "CUserCmd.h"
#define MULTIPLAYER_BACKUP 150

class IInput
{
private:
	char u0[172];
	bool u1;
public:
	bool CameraInThirdPerson;
private:
	char u2[2];
public:
	Vector CameraOffset;

	CUserCmd* GetUserCMD(int slot, int sequence_num)
	{
		using fn = CUserCmd* (__thiscall*)(void*, int, int);
		return (*(fn**)this)[8](this, slot, sequence_num);
	}
};