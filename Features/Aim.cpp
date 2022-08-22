#include "Aim.h"

#include "../Game.h"
#include "../SDK/Interfaces/CUserCmd.h"
#include "Variables.h"

namespace Aim
{
	void RCS(CUserCmd* cmd)
	{
		if (!C::Get<bool>(Vars.AIM_RCS))
			return;

		/*
		The current recoil angles can be found in a NetVar inside the local player entity.

		By subtracting these angles from the current view angles, weapon recoil can be reduced by a certain extent or compensated completely.
		*/

		// m_aimPunchAngle 

		cmd->viewangles -= Game::LocalPlayer->aim_punch_angle();
		cmd->viewangles.Normalized();
	}

	CBasePlayer* GetTarget(CUserCmd* cmd)
	{
		CBasePlayer* Target = nullptr;
		float Delta;
		float BestDelta = FLT_MAX;

		for (int i = 1; i < Interfaces::g_Engine->GetMaxClients(); i++)
		{
			CBasePlayer* Player = reinterpret_cast<CBasePlayer*>(Interfaces::g_EntityList->GetClientEntity(i));

			if (Player == nullptr)
				continue;

			if (Player == Game::LocalPlayer)
				continue;

			if (!Player->IsAlive())
				continue;

			if (Player->has_gun_game_immunity())
				continue;

			if (Player->team() == Game::LocalPlayer->team())
				continue;

			if (Game::LocalPlayer->IsFlashed())
				continue;

			if (Game::LocalPlayer->CanSeePlayerPos(Game::LocalPlayer, Player->GetEyePos()))
				continue;

			Delta = Math::CalcAimAngle(Game::LocalPlayer->GetEyePos(), Player->GetEyePos(), cmd->viewangles).Length();

			if (Delta < BestDelta && Delta < C::Get<float>(Vars.AIM_FOV))
			{
				Target = Player;
				AimTarget = Target;
				BestDelta = Delta;
			}
		}
		return Target;
	}

	void Run(CUserCmd* cmd)
	{
		if (!Interfaces::g_Engine->IsConnected())
			return;

		RCS(cmd);

		if (!C::Get<bool>(Vars.AIM_ENABLED))
			return;

		//Hitchance = 1 / Game::LocalPlayer->ActiveWeapon()->Inaccuracy();

		CBasePlayer* Target = GetTarget(cmd);

		int Hitbox = 1;
		if (C::Get<bool>(Vars.AIM_SMARTHITBOX))
		{

		}
		else
		{
			if (C::Get<int>(Vars.AIM_HITBOX_SELECTION) == 0)
				Hitbox = 1;
			else
				Hitbox = 5;
		}

		AimAngle = Math::CalcAimAngle(Game::LocalPlayer->GetEyePos(), Target->GetHitboxPosition(Hitbox), cmd->viewangles);

		if (AimAngle.IsValid())
		{
			if (&C::Get<bool>(Vars.AIM_VISCHECK) && !Target->CanSeePlayerPos(Game::LocalPlayer, Game::LocalPlayer->GetEyePos()))
				return;

			AimAngle.Normalized();
			cmd->viewangles += AimAngle;

			if (!C::Get<bool>(Vars.AIM_SILENTAIM))
			{
				AimAngle /= (C::Get<float>(Vars.AIM_SMOOTHAMOUNT) * 4);
				Interfaces::g_Engine->SetViewAngles(&cmd->viewangles);
			}
		}
		else
		{
			AimAngle.Invalidate();
		}
	}
}
