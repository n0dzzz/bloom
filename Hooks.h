#pragma once
#include <d3d9.h>
#include <d3d9types.h>

#include "Dependencies/Minhook/MinHook.h"
#include "SDK/Interfaces/CUserCmd.h"
#include "SDK/Interfaces/IPanel.h"
#include "Utilities/Utils.h"
#include <windows.h>



#include "SDK/Interfaces/IVModelRender.h"
#include "SDK/Structs/Materials.h"
#include "SDK/Structs/Models.h"

namespace Hooks
{

	enum client_frame_stage_t
	{
		FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
		FRAME_START,

		// A network packet is being recieved
		FRAME_NET_UPDATE_START,
		// Data has been received and we're going to start calling PostDataUpdate
		FRAME_NET_UPDATE_POSTDATAUPDATE_START,
		// Data has been received and we've called PostDataUpdate on all data recipients
		FRAME_NET_UPDATE_POSTDATAUPDATE_END,
		// We've received all packets, we can now do interpolation, prediction, etc..
		FRAME_NET_UPDATE_END,

		// We're about to start rendering the scene
		FRAME_RENDER_START,
		// We've finished rendering the scene.
		FRAME_RENDER_END
	};


	inline int watermarkx = 97;
	inline int watermarky = 10;
	inline int watermarkw = 350;
	inline int watermarkh = 80;
	
	bool Init();
	void Detatch();
	
	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace CreateMove
	{
		using fn = bool(__stdcall*)(float, CUserCmd*);
		bool __stdcall hook(float input_sample_frametime, CUserCmd* cmd);
	};

	namespace PaintTraverse
	{
		using fn = void(__thiscall*)(IPanel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}
	
	namespace EndScene
	{
		using fn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*);
		HRESULT __stdcall hook(IDirect3DDevice9* device);
	}

	namespace SceneReset
	{
		using fn = long(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
	}

	namespace ViewmodelFOV
	{
		using fn = float(__thiscall*)(void*);
		float __fastcall hook(PVOID convar);
	}
	
	namespace DrawModelExecute
	{
		using fn = void(__fastcall*)(void*, int, MatRenderContext*, const DrawModelState&, const ModelRenderInfo&, Matrix*);
		void __fastcall hook(void* _this, int edx, MatRenderContext* ctx, const DrawModelState& state, const ModelRenderInfo& pInfo, Matrix* pCustomBoneToWorld);
	}

	namespace FrameStageNotify
	{
		using fn = void(__thiscall*)(void*, client_frame_stage_t);
		void __stdcall hook(client_frame_stage_t FrameStage);
	}
}
