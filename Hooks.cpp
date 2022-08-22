#include "Hooks.h"

#include <algorithm>


#include "Game.h"
#include "Menu.h"
#include "SDK/SDK.h"
#include "SDK/Classes/Entities.h"
#include "SDK/Misc/FNV.h"

#include "Features/Misc.h"
#include "Utilities/Render/Render.h"

#include <tchar.h>
#include <windows.h>

#include "Features/Visuals.h"
#include "Dependencies/ImGui/imgui_impl_dx9.h"
#include "Dependencies/ImGui/imgui_impl_win32.h"
#include "Features/Aim.h"
#include "Features/Variables.h"
#include "Utilities/Config/Config.h"
#include "Utilities/Render/BloomUI/BloomUI.h"

Hooks::CreateMove::fn CreateMoveOrigin = nullptr;
Hooks::PaintTraverse::fn PaintTraverseOrigin = nullptr;
Hooks::ViewmodelFOV::fn ViewModelOrigin = nullptr;
Hooks::DrawModelExecute::fn DrawModelExecuteOrigin = nullptr;
Hooks::FrameStageNotify::fn FrameStageNotifyOrigin = nullptr;

Hooks::EndScene::fn EndSceneOrigin = nullptr;
Hooks::SceneReset::fn ResetOrigin = nullptr;


namespace Hooks
{
	bool Init()
	{
		const auto CreateMoveTarget = reinterpret_cast<void*>(get_virtual(Interfaces::g_ClientMode, 24));
		const auto PaintTraverseTarget = reinterpret_cast<void*>(get_virtual(Interfaces::g_Panel, 41));
		const auto EndSceneTarget = reinterpret_cast<void*>(get_virtual(Interfaces::g_D3DDevice9, 42));
		const auto ResetTarget = reinterpret_cast<void*>(get_virtual(Interfaces::g_D3DDevice9, 16));
		const auto ViewmodelTarget = reinterpret_cast<void*>(get_virtual(Interfaces::g_ClientMode, 35));
		const auto DrawModelExecuteTarget = reinterpret_cast<void*>(get_virtual(Interfaces::g_MdlRender, 21));
		const auto FrameStageNotifyTarget = reinterpret_cast<void*>(get_virtual(Interfaces::g_CHLClient, 37));

		if (MH_Initialize() != MH_OK)
			throw std::runtime_error("{HOOKS} Failed to Init - MH_Initialize");

		if (MH_CreateHook(CreateMoveTarget, &CreateMove::hook, reinterpret_cast<void**>(&CreateMoveOrigin)) != MH_OK)
			throw std::runtime_error("failed to initialize create_move. (outdated index?)");

		if (MH_CreateHook(PaintTraverseTarget, &PaintTraverse::hook, reinterpret_cast<void**>(&PaintTraverseOrigin)) != MH_OK)
			throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

		if (MH_CreateHook(EndSceneTarget, &EndScene::hook, reinterpret_cast<void**>(&EndSceneOrigin)) != MH_OK)
			throw std::runtime_error("failed to initialize EndScene. (outdated index?)");

		if (MH_CreateHook(ResetTarget, &SceneReset::hook, reinterpret_cast<void**>(&ResetOrigin)) != MH_OK)
			throw std::runtime_error("failed to initialize SceneReset. (outdated index?)");

		if (MH_CreateHook(FrameStageNotifyTarget, &FrameStageNotify::hook, reinterpret_cast<void**>(&FrameStageNotifyOrigin)) != MH_OK)
			throw std::runtime_error("failed to initialize Frame Stage Notify. (outdated index?)");

		if (MH_CreateHook(ViewmodelTarget, &ViewmodelFOV::hook, reinterpret_cast<void**>(&ViewModelOrigin)) != MH_OK)
			throw std::runtime_error("failed to initialize View Model Override. (outdated index?)");
		
		if (MH_CreateHook(DrawModelExecuteTarget, &DrawModelExecute::hook, reinterpret_cast<void**>(&DrawModelExecuteOrigin)) != MH_OK)
			throw std::runtime_error("failed to initialize Draw Model Execute. (outdated index?)");

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			throw std::runtime_error("{HOOKS} Failed to Enable - Hooks");

		Utils::ConsolePrint("{HOOKS} Success - Init Success", 3);

		return true;
	}

	void Detatch()
	{
		MH_Uninitialize();

		ImGuiMenu::Release();

		MH_DisableHook(MH_ALL_HOOKS);
	}

	bool __stdcall CreateMove::hook(float input_sample_frametime, CUserCmd* cmd)
	{
		CreateMoveOrigin(input_sample_frametime, cmd);

		if (!cmd || !cmd->command_number)
			return false;

		Game::LocalPlayer = reinterpret_cast<CBasePlayer*>(Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer()));
		
		uintptr_t* FramePointer;
		__asm mov FramePointer, ebp;
		bool& send_packet = *reinterpret_cast<bool*>(*FramePointer - 0x1C);

		auto OldViewangles = cmd->viewangles;
		auto OldForwardMove = cmd->forwardmove;
		auto OldSidemove = cmd->sidemove;
		
		
		Aim::Run(cmd);
		Misc::BunnyHop(cmd);
		Misc::PlayerLookingInfo();

		Math::CorrectMove(OldViewangles, cmd, OldForwardMove, OldSidemove);

		cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
		cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
		cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

		cmd->viewangles.Normalized();
		cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
		cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
		cmd->viewangles.z = 0.0f;
		
		return false;
	}

	void __stdcall PaintTraverse::hook(unsigned panel, bool force_repaint, bool allow_force)
	{
		auto panel_to_draw = Fnv::hash(Interfaces::g_Panel->GetName(panel));

		switch (panel_to_draw)
		{
		case Fnv::hash("MatSystemTopPanel"):
		{
			// Render Here
			Misc::DebugInfo();
			Misc::RecoilCrosshair();

			if (C::Get<bool>(Vars.MISC_Watermark))
			{
				Render::FilledRect(watermarkx, watermarky, watermarkw, watermarkh, Color(25, 25, 25, 125));
				Render::Rect(watermarkx, watermarky, watermarkw, watermarkh, Color(53, 136, 25, 125));
				Render::Text(watermarkx + 3, watermarky + 30, Render::Fonts::DefaultFont, false, "BLOOM", Color(255, 255, 255));

				#ifdef _DEBUG
					Render::Text(watermarkx + 3, watermarky + 55, Render::Fonts::DefaultFont, false, "Debug", Color(255, 255, 255, 255));
				#endif

				BloomUI::MoveMenu(watermarkx, watermarky, watermarkw, watermarkh);
			}			
	
			
			Visuals::Render();

			//BloomUI::Toggle();
			//Menu::Render();
			Misc::SpotifyDisplay();

			break;
		}
		case Fnv::hash("FocusOverlayPanel"):
			Interfaces::g_Panel->SetMouseInputEnabled(panel, ImGuiMenu::IsVisible);
			Interfaces::g_Panel->SetKeyboardInputEnabled(panel, ImGuiMenu::IsVisible);
			break;
		}

		PaintTraverseOrigin(Interfaces::g_Panel, panel, force_repaint, allow_force);
	}

	HRESULT __stdcall EndScene::hook(IDirect3DDevice9* device)
	{
		if (!ImGuiMenu::Initialiazed)
			ImGuiMenu::Init(device);

		ImGuiMenu::Toggle();
		
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (ImGuiMenu::IsVisible)
			ImGuiMenu::Render();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		
		return EndSceneOrigin(Interfaces::g_D3DDevice9, device);
	}

	long __stdcall SceneReset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		ImGui_ImplDX9_CreateDeviceObjects();
		return ResetOrigin(Interfaces::g_D3DDevice9, device, params);
	}

	float __fastcall ViewmodelFOV::hook(PVOID convar)
	{
		return ViewModelOrigin(convar) + C::Get<float>(Vars.MISC_FovOverride);
	}

	void __fastcall DrawModelExecute::hook(void* _this, int edx, MatRenderContext* ctx, const DrawModelState& state, const ModelRenderInfo& pInfo, Matrix* pCustomBoneToWorld)
	{
		if (Interfaces::g_Engine->IsInGame() && Game::LocalPlayer)
		{
			const auto mdl = pInfo.model;

			if (!mdl)
				return;

			bool IsPlayer = strstr(mdl->name, "models/player") != nullptr;

			if (!Interfaces::g_StudioRender->IsForced() && IsPlayer)
			{
				Visuals::Chams(ctx, state, pInfo, pCustomBoneToWorld);
				DrawModelExecuteOrigin(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
				Interfaces::g_MdlRender->OverrideMaterial(nullptr);
			}
			else if (!Interfaces::g_MdlRender->IsForcedMaterialOverride() && IsPlayer)
			{
				Visuals::Chams(ctx, state, pInfo, pCustomBoneToWorld);
				DrawModelExecuteOrigin(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
				Interfaces::g_MdlRender->OverrideMaterial(nullptr);
			}
			else
			{
				DrawModelExecuteOrigin(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
			}

		}
	}

	void __stdcall Hooks::FrameStageNotify::hook(client_frame_stage_t FrameStage)
	{
		const auto local = reinterpret_cast<CBasePlayer*>(Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer()));
		static auto set_interpolation_flags = [](CBasePlayer* e, int flag)
		{
			const auto var_map = (uintptr_t)e + 36;
			const auto sz_var_map = *(int*)(var_map + 20);
			for (auto index = 0; index < sz_var_map; index++)
				*(uintptr_t*)((*(uintptr_t*)var_map) + index * 12) = flag;
		};

		switch (FrameStage)
		{
		case FRAME_UNDEFINED:
			break;
		case FRAME_START:
			break;
		case FRAME_NET_UPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			break;
		case FRAME_NET_UPDATE_END:
			break;
		case FRAME_RENDER_START:

			if (local)
			{
				Misc::Thirdperson();
				if (!local->IsAlive())
					Interfaces::g_Input->CameraInThirdPerson = false;
				if (local->IsAlive() && Interfaces::g_Input->CameraInThirdPerson)
					*(Vector*)(((DWORD)local) + 0x31D4 + 0x4) = Game::Angles;
			}

			break;
		case FRAME_RENDER_END:
			break;
		default: break;
		}
		FrameStageNotifyOrigin(Interfaces::g_CHLClient, FrameStage);
	}
}