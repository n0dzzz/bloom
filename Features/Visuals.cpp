#include "Visuals.h"
#include "../Hooks.h"

extern Hooks::DrawModelExecute::fn DrawModelExecuteOrigin;

struct Box
{
	int x, y, w, h;
	Box() = default;
	Box(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};

std::string CleanItemName(std::string name)
{

	std::string Name = name;

	auto WeaponStart = Name.find("weapon");

	if (WeaponStart != std::string::npos)
		Name.erase(Name.begin() + WeaponStart, Name.begin() + WeaponStart + 6);

	if (Name[0] == '_')
		Name.erase(Name.begin());

	if (Name[0] == 'c')
		Name.erase(Name.begin());

	return Name;
}

bool GetPlayerbox(CBasePlayer* Entity, Box& In)
{
	Vector origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	origin = Entity->ABSOrigin();
	min = Entity->Collideable()->Mins() + origin;
	max = Entity->Collideable()->Maxs() + origin;

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	if (Math::WorldToScreen(points[3], flb) || Math::WorldToScreen(points[5], brt)
		|| Math::WorldToScreen(points[0], blb) || Math::WorldToScreen(points[4], frt)
		|| Math::WorldToScreen(points[2], frb) || Math::WorldToScreen(points[1], brb)
		|| Math::WorldToScreen(points[6], blt) || Math::WorldToScreen(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	In.x = (int)left;
	In.y = (int)top;
	In.w = int(right - left);
	In.h = int(bottom - top);

	return true;
}

namespace Visuals
{
	void SkeletonESP(CBasePlayer* Player)
	{
		if (!C::Get<bool>(Vars.ESP_Skeleton))
			return;

		// Player = target player
		// CanSeePlayerPos, two params, 1st param is the player that we want to know if the target can see, and the 2nd param is the position of the player we want to know if the target can see 
		// "facts" - jonn
		if (!C::Get<bool>(Vars.ESP_SkeletonXQZ) && !Player->CanSeePlayerPos(Game::LocalPlayer, Game::LocalPlayer->GetEyePos()))
			return;

		auto StudioModel = Interfaces::g_ModelInfo->GetStudiomodel(Player->MModel());

		if (!StudioModel)
			return;

		static Matrix BoneToWorldOut[128];

		if (Player->SetupBones(BoneToWorldOut, 128, 256, 0))
		{
			for (int i = 0; i < StudioModel->bones_count; i++)
			{
				StudioBone* Bone = StudioModel->bone(i);

				if (!Bone || !(Bone->flags & 256) || Bone->parent == -1)
					continue;

				Vector BonePosition1;

				if (!Math::WorldToScreen(Vector(BoneToWorldOut[i][0][3], BoneToWorldOut[i][1][3], BoneToWorldOut[i][2][3]), BonePosition1))
					continue;

				Vector BonePosition2;

				if (!Math::WorldToScreen(Vector(BoneToWorldOut[Bone->parent][0][3], BoneToWorldOut[Bone->parent][1][3], BoneToWorldOut[Bone->parent][2][3]), BonePosition2))
					continue;

				Render::DrawLine(BonePosition1.x, BonePosition2.x, BonePosition1.y, BonePosition2.y, C::Get<Color>(Vars.ESP_SkeletonColor));
			}
		}

	}

	void Render()
	{
		if (!Interfaces::g_Engine->IsInGame())
			return;

		for (int iPlayer = 0; iPlayer < Interfaces::g_Engine->GetMaxClients(); iPlayer++)
		{
			if (Game::LocalPlayer == nullptr)
				return;

			auto Player = reinterpret_cast<CBasePlayer*>(Interfaces::g_EntityList->GetClientEntity(iPlayer));

			if (Player == Game::LocalPlayer || !Player->IsAlive() || Player->Dormant())
				continue;

			if (Player->team() == Game::LocalPlayer->team() && C::Get<bool>(Vars.ESP_TeamCheck))
				continue;

			Vector VecFoot;
			Vector VecScreen;
			Vector VecHeadScreen;
			VecFoot = Player->origin();
			if (!(Math::WorldToScreen(VecFoot, VecScreen)))
				continue;

			VecFoot.z += 72.f;
			if (!(Math::WorldToScreen(VecFoot, VecHeadScreen)))
				continue;

			player_info_t PlayerInfo;
			Interfaces::g_Engine->GetPlayerInfo(iPlayer, &PlayerInfo);

			int h = VecScreen.y - VecHeadScreen.y;
			int w = (h / 2);
			int y = VecHeadScreen.y;
			int x = VecHeadScreen.x - w / 2;

			int HealthHeight = h * (std::clamp(Player->health(), 0, 100) / 100.f) - (Player->health() >= 100 ? 0 : -1);

			SkeletonESP(Player);

			if (C::Get<bool>(Vars.MISC_AntiFlash))
			{
				if (Game::LocalPlayer->flash_alpha() > 0)
					Game::LocalPlayer->flash_alpha() = 0;
			}

			if (C::Get<bool>(Vars.ESP_Box))
			{
				Color BoxColor = C::Get<Color>(Vars.ESP_BoxColor);

				if (C::Get<bool>(Vars.ESP_AimTargetBoxOverride) && Player == Aim::AimTarget)
					BoxColor = C::Get<Color>(Vars.ESP_AimTargetBoxOverrideColor);

				Render::Rect(x, y, w, h, Color(0, 0, 0));
				Render::Rect(x + 1, y + 1, w - 2, h - 2, BoxColor);
			}

			if (C::Get<bool>(Vars.ESP_Names))
				Render::Text(VecHeadScreen.x, VecHeadScreen.y - 20, Render::Fonts::EspFont, true, PlayerInfo.szName, C::Get<Color>(Vars.ESP_NamesColor));

			if (C::Get<bool>(Vars.ESP_HealthBar))
			{
				Color HealthColor = Color(3, 252, 119);

				if (Player->health() < 60)
					HealthColor = Color(168, 162, 50);

				if (Player->health() < 35)
					HealthColor = Color(168, 50, 75);

				Render::Rect(VecHeadScreen.x - w / 1.8, VecHeadScreen.y, 4, HealthHeight, Color(0, 0, 0));
				Render::FilledRect(VecHeadScreen.x - w / 1.8 + 1, VecHeadScreen.y + 1, 4 - 2, HealthHeight - 2, HealthColor);
			}

			std::vector<std::pair<std::string, Color>> Flags;

			if (C::Get<bool>(Vars.FLAGS_WeaponNames))
				Flags.emplace_back(std::pair<std::string, Color>(CleanItemName(Player->ActiveWeapon()->GetWeaponData()->weapon_name), Color(255, 255, 255)));

			if (C::Get<bool>(Vars.FLAGS_Armor) && Player->has_helmet() && Player->armor() > 0)
				Flags.emplace_back(std::pair<std::string, Color>("HK", Color(83, 142, 237)));
			else if (C::Get<bool>(Vars.FLAGS_Armor) && !Player->has_helmet() && Player->armor() > 0)
				Flags.emplace_back(std::pair<std::string, Color>("K", Color(83, 142, 237)));

			if (C::Get<bool>(Vars.FLAGS_Money) && Player->money())
				Flags.emplace_back(std::pair<std::string, Color>(std::to_string(Player->money()).insert(0, "$"), Color(64, 227, 94)));

			if (C::Get<bool>(Vars.FLAGS_IsFlashed) && Player->IsFlashed())
				Flags.emplace_back(std::pair<std::string, Color>("Flashed", Color(227, 222, 64)));

			if (C::Get<bool>(Vars.FLAGS_DefuseKit) && Player->has_defuser())
				Flags.emplace_back(std::pair<std::string, Color>("Defuse Kit", Color(64, 189, 227)));

			if (C::Get<bool>(Vars.FLAGS_Defusing) && Player->is_defusing())
				Flags.emplace_back(std::pair<std::string, Color>("Defusing", Color(227, 64, 94)));

			if (C::Get<bool>(Vars.FLAGS_Invincible) && Player->has_gun_game_immunity())
				Flags.emplace_back(std::pair<std::string, Color>("Invincible", Color(156, 13, 181)));

			if (C::Get<bool>(Vars.FLAGS_AimTarget) && Player == Aim::AimTarget)
				Flags.emplace_back(std::pair<std::string, Color>("Aim Target", Color(163, 13, 13)));

			auto Position = 0;
			for (auto Flag : Flags)
			{
				int Right, Bottom;
				const auto ConvertedText = std::wstring(Flag.first.begin(), Flag.first.end());
				Interfaces::g_Surface->GetTextSize(Render::Fonts::EspFont, ConvertedText.c_str(), Right, Bottom);

				Render::Text(x + w + 10 + (Right / 2), y + Position, Render::Fonts::EspFont, true, Flag.first, Flag.second);
				Position += 10;
			}

		}
	}

	void OverrideMaterial(bool ignorez, bool wireframe, const Color& rgba)
	{
		auto Material = Interfaces::g_MatSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
		Material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignorez);
		Material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
		Material->AlphaModulate(rgba.a() / 255.f);
		Material->ColorModulate(rgba.r() / 255.f, rgba.g() / 255.f, rgba.b() / 255.f);
		Interfaces::g_MdlRender->OverrideMaterial(Material);
	}

	void Chams(MatRenderContext* ctx, const DrawModelState& state, const ModelRenderInfo& info, Matrix* matrix)
	{
		if (!C::Get<bool>(Vars.CHAMS_Enabled))
			return;

		const auto MDL = info.model;

		bool IsPlayer = strstr(MDL->name, "models/player") != nullptr;

		if (IsPlayer)
		{
			CBasePlayer* Player = reinterpret_cast<CBasePlayer*>(Interfaces::g_EntityList->GetClientEntity(info.entity_index));

			if (!Player || !Player->IsAlive() || Player->Dormant())
				return;

			if (Player->team() == Game::LocalPlayer->team() && C::Get<bool>(Vars.CHAMS_TeamCheck))
				return;

			if (Player->has_gun_game_immunity())
			{
				OverrideMaterial(false, false, Color(255, 255, 255, 100));
				DrawModelExecuteOrigin(Interfaces::g_MdlRender, 0, ctx, state, info, matrix);
			}

			if (C::Get<int>(Vars.CHAMS_Type) == 0) // non xqz
			{
				OverrideMaterial(false, false, C::Get<Color>(Vars.CHAMS_VisibleColor));
				DrawModelExecuteOrigin(Interfaces::g_MdlRender, 0, ctx, state, info, matrix);
			}

			if (C::Get<int>(Vars.CHAMS_Type) == 1) // xqz
			{

				OverrideMaterial(true, false, C::Get<Color>(Vars.CHAMS_XQZColor));
				DrawModelExecuteOrigin(Interfaces::g_MdlRender, 0, ctx, state, info, matrix);

				OverrideMaterial(false, false, C::Get<Color>(Vars.CHAMS_VisibleColor));
				DrawModelExecuteOrigin(Interfaces::g_MdlRender, 0, ctx, state, info, matrix);
			}

		}
	}

}
