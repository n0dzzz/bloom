#pragma once

#define NOMINMAX
#include <d3d9.h>
#include <Windows.h>

#include "Interfaces/IRenderView.h"
#include "Interfaces/ISurface.h"
#include "Interfaces/IClientMode.h"
#include "Interfaces/IPanel.h"
#include "Interfaces/ISurface.h"
#include "Interfaces/IBaseClientDll.h"
#include "Interfaces/IVEngineClient.h"
#include "Interfaces/IWeaponSystem.h"
#include "Interfaces/IEngineTrace.h"
#include "Interfaces/IClientEntityList.h"
#include "Interfaces/IVModelInfo.h"
#include "../Dependencies/ImGui/imgui.h"
#include "Interfaces/IVModelRender.h"
#include "Interfaces/IStudioRender.h"
#include "Interfaces/IInput.h"

namespace Interfaces
{
	void Init();
	void Dump();

	inline IVRenderView* g_RenderView = nullptr;
	inline IDirect3DDevice9* g_D3DDevice9 = nullptr;
	inline ISurface* g_Surface = nullptr;
	inline IPanel* g_Panel = nullptr;
	inline IClientMode* g_ClientMode = nullptr;
	inline IBaseClientDLL* g_CHLClient = nullptr;
	inline IVEngineClient* g_Engine = nullptr;
	inline IWeaponSystem* g_WeaponSystem = nullptr;
	inline IEngineTrace* g_TraceRay = nullptr;
	inline IClientEntityList* g_EntityList = nullptr;
	inline IVModelInfo* g_ModelInfo = nullptr;
	inline CGlobalVarsBase* g_Globals;
	inline IVModelRender* g_MdlRender = nullptr;
	inline IMaterialSystem* g_MatSystem = nullptr;
	inline IStudioRender* g_StudioRender = nullptr;
	inline IInput* g_Input = nullptr;
}
