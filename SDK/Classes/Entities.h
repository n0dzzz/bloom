#pragma once
#include <cstdint>

#include "../../Utilities/Netvars.h"
#include "../Math/Vector.h"
#include "../../Utilities/Utils.h"
#include "Collideable.h"
#include "../../Utilities/Math/Math.h"
#include "../Interfaces/ClientClass.h"
#include "../Interfaces/IEngineTrace.h"
#include "../Structs/Animstate.h"
#include "../Structs/Models.h"
#include "../Structs/WeaponInfo.h"
#include "../SDK.h"

class CBaseEntity
{
public:
	
	model_t* MModel()
	{
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)Animating())[8](Animating());
	}
	
	void* Animating()
	{
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
	void* Networkable()
	{
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	CCollideable* Collideable()
	{
		using original_fn = CCollideable * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}
	ClientClass* FClientClass()
	{
		using original_fn = ClientClass * (__thiscall*)(void*);
		return (*(original_fn**)Networkable())[2](Networkable());
	}

	int Index()
	{
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)Networkable())[10](Networkable());
	}
	bool IsPlayer()
	{
		using original_fn = bool(__thiscall*)(CBaseEntity*);
		return (*(original_fn**)this)[157](this);
	}
	bool IsWeapon()
	{
		using original_fn = bool(__thiscall*)(CBaseEntity*);
		return (*(original_fn**)this)[165](this);
	}
	bool SetupBones(Matrix* out, int max_bones, int mask, float time)
	{
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, Matrix*, int, int, float);
		return (*(original_fn**)Animating())[13](Animating(), out, max_bones, mask, time);
	}
	Model* FModel()
	{
		using original_fn = Model * (__thiscall*)(void*);
		return (*(original_fn**)Animating())[8](Animating());
	}
	void Update()
	{
		using original_fn = void(__thiscall*)(CBaseEntity*);
		(*(original_fn**)this)[218](this);
	}
	int DrawModel(int flags, uint8_t alpha)
	{
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn**)Animating())[9](Animating(), flags, alpha);
	}
	
	void SetAngles(Vector angles)
	{
		using original_fn = void(__thiscall*)(void*, const Vector&);
		static original_fn set_angles_fn = (original_fn)((DWORD)Utils::PatternScan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}

	void SetPosition(Vector position)
	{
		using original_fn = void(__thiscall*)(void*, const Vector&);
		static original_fn set_position_fn = (original_fn)((DWORD)Utils::PatternScan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}

	void SetModelIndex(int index)
	{
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}

	void NetPreDataUpdate(int update_type)
	{
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)Networkable())[6](Networkable(), update_type);
	}

	void NetRelease()
	{
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)Networkable())[1](Networkable());
	}

	int NetSetDestroyedOnRecreateEntities()
	{
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)Networkable())[13](Networkable());
	}

	bool Dormant()
	{
		using original_fn = bool(__thiscall*)(void*);
		return (*static_cast<original_fn**>(Networkable()))[9](Networkable());
	}

		NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
		NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long)
		NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float)
		NETVAR("DT_BasePlayer", "m_vecOrigin", origin, Vector)
		NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, Vector)
		NETVAR("DT_CSPlayer", "m_iTeamNum", team, int)
		NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool)
		NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survival_team, int)
		NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, float)
};

class CBaseWeapon : public CBaseEntity
{
public:
		NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", NextPrimaryAttack, float)
		NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", NextSecondaryAttack, float)
		NETVAR("DT_BaseCombatWeapon", "m_iClip1", Clip1, int)
		NETVAR("DT_BaseCombatWeapon", "m_iClip2", Clip2, int)
		NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", PrimaryReserveAmmoAccount, int)
		NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", RecoilIndex, float)
		NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", ZoomLevel, float)
		NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", ItemDefIndex, short)
		NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", EntityQuality, int)
		NETVAR("DT_BaseCombatWeapon", "m_nFallbackPaintKit", FallbackPaintKit, int)
		NETVAR("DT_BaseCombatWeapon", "m_nFallbackSeed", FallbackSeed, int)
		NETVAR("DT_BaseCombatWeapon", "m_nFallbackStatTrak", FallbackStatTrak, int)
		NETVAR("DT_BaseCombatWeapon", "m_flFallbackWear", FallbackWear, int)
		NETVAR("DT_BaseCombatWeapon", "m_iItemIDHigh", ItemIDHigh, int)

		enum CSWeaponType
		{
			WEAPONTYPE_KNIFE = 0,
			WEAPONTYPE_PISTOL,
			WEAPONTYPE_SUBMACHINEGUN,
			WEAPONTYPE_RIFLE,
			WEAPONTYPE_SHOTGUN,
			WEAPONTYPE_SNIPER_RIFLE,
			WEAPONTYPE_MACHINEGUN,
			WEAPONTYPE_C4,
			WEAPONTYPE_PLACEHOLDER,
			WEAPONTYPE_GRENADE,
			WEAPONTYPE_UNKNOWN
		};

	float GetSpread()
	{
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[452](this);
	}

	float Inaccuracy()
	{
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[482](this);
	}

	void UpdateAccuracyPenalty()
	{
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[483](this);
	}

	CBaseWeaponInfo* GetWeaponData()
	{
		return Interfaces::g_WeaponSystem->GetWeaponData(this->ItemDefIndex());
	}
};

class CBasePlayer : public CBaseEntity
{
private:
	template <typename T>
	T& read(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data)
	{
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int)
		NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool)
		NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool)
		NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int)
		NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, Vector)
		NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int)
		NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool)
		NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool)
		NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool)
		NETVAR("DT_CSPlayer", "m_iAccount", money, int)
		NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float)
		NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float)
		NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float)
		NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float)
		NETVAR("DT_CSPlayer", "m_bHasNightVision", has_night_vision, bool)
		NETVAR("DT_CSPlayer", "m_bNightVisionOn", night_vision_on, bool)
		NETVAR("DT_CSPlayer", "m_iHealth", health, int)
		NETVAR("DT_CSPlayer", "m_lifeState", life_state, int)
		NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
		NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, Vector)
		NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, Vector)
		NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, Vector)
		NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float)
		NETVAR("DT_BaseEntity", "m_flShadowCastDistance", fov_time, float)
		NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long)
		NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int)
		NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float)
		NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool)
		NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int)
		NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int)

		CBaseWeapon* ActiveWeapon()
	{
		auto active_weapon = read<uintptr_t>(NetvarManager::GetNetVar(Fnv::hash("DT_CSPlayer"), Fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<CBaseWeapon*>(Interfaces::g_EntityList->GetClientEntity(active_weapon));
	}

	UINT* GetWearables()
	{
		return (UINT*)((uintptr_t)this + (NetvarManager::GetNetVar(Fnv::hash("DT_CSPlayer"), Fnv::hash("m_hMyWearables"))));
	}

	UINT* GetWeapons()
	{
		return (UINT*)((uintptr_t)this + (NetvarManager::GetNetVar(Fnv::hash("DT_CSPlayer"), Fnv::hash("m_hMyWeapons"))));
	}

	Vector GetEyePos()
	{
		return origin() + view_offset();
	}

	Animstate* GetAnimState()
	{
		return *reinterpret_cast<Animstate**>(this + 0x3914);
	}

	bool CanSeePlayerPos(CBasePlayer* player, const Vector& pos)
	{
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto start = GetEyePos();
		auto dir = (pos - start).Normalized();

		ray.initialize(start, pos);
		Interfaces::g_TraceRay->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.flFraction > 0.97f;
	}

	Vector GetBonePosition(int bone)
	{
		Matrix bone_matrices[128];
		if (SetupBones(bone_matrices, 128, 256, 0.0f))
			return Vector{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return Vector{ };
	}

	Vector GetHitboxPosition(int hitbox_id)
	{
		Matrix boneMatrix[MAXSTUDIOBONES];

		if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
		{
			auto studio_model = Interfaces::g_ModelInfo->GetStudiomodel(MModel());
			if (studio_model)
			{
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);
				if (hitbox)
				{
					auto
						min = Vector{},
						max = Vector{};

					Math::TransformVectors(hitbox->mins, boneMatrix[hitbox->bone], min);
					Math::TransformVectors(hitbox->maxs, boneMatrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
		return Vector{};
	}

	bool IsAlive()
	{
		if (!this) return false;
		return this->health() > 0;
	}

	bool IsMoving()
	{
		if (!this) return false;
		return this->velocity().Length() > 0.1f;
	}

	bool IsInAir()
	{
		if (!this) return false;
		return !(this->flags() & FL_ONGROUND);
	}

	bool IsFlashed()
	{
		if (!this) return false;
		return this->flash_duration() > 0.0f;
	}

	void UpdateClientSideAnimations()
	{
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[223](this);
	}

	Vector& ABSOrigin()
	{
		using original_fn = Vector & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}
	Vector& ABSAngles()
	{
		using original_fn = Vector & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);;
	}

	int MoveType()
	{
		static int type = NetvarManager::GetNetVar(Fnv::hash("DT_BaseEntity"), Fnv::hash("m_nRenderMode")) + 1;
		return read<int>(type);
	}
};
