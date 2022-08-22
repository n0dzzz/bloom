#include "SDK.h"

#include "../Utilities/Utils.h"
#include "Interfaces/IVModelInfo.h"
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

namespace Interfaces
{

    CreateInterfaceFn get_module_factory(HMODULE module)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
    }

    template<typename T>
    T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
    {
        auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

        if (!result)
        {
            throw std::runtime_error(std::string("{INTERFACES} Failed - GetOffset Interface") + szInterfaceVersion);
        }

        return result;
    }
	
	void Init()
	{
        auto engineFactory = get_module_factory(GetModuleHandleW(L"engine.dll"));
    	
        auto dx9api = GetModuleHandleW(L"shaderapidx9.dll");
    	
        auto vguiFactory = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll"));

        auto vgui2Factory = get_module_factory(GetModuleHandleW(L"vgui2.dll"));

        auto clientFactory = get_module_factory(GetModuleHandleW(L"client.dll"));
    	
        auto matSysFactory = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));

        auto studioRenderFactory = get_module_factory(GetModuleHandleW(L"studiorender.dll"));
    	
        g_TraceRay = get_interface<IEngineTrace>(engineFactory, "EngineTraceClient004");
    	
        g_StudioRender = get_interface<IStudioRender>(studioRenderFactory,"VStudioRender026");
    	
        g_EntityList = get_interface<IClientEntityList>(clientFactory, "VClientEntityList003");

        g_ModelInfo = get_interface<IVModelInfo>(engineFactory, "VModelInfoClient004");
    	
        g_CHLClient = get_interface<IBaseClientDLL>(clientFactory, "VClient018");
    	
        g_RenderView = get_interface<IVRenderView>(engineFactory, "VEngineRenderView014");

        g_D3DDevice9 = **(IDirect3DDevice9***)(Utils::PatternScan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

        g_MdlRender = get_interface<IVModelRender>(engineFactory, "VEngineModel016");
    	
        g_MatSystem = get_interface<IMaterialSystem>(matSysFactory, "VMaterialSystem080");
    	
        g_Surface = get_interface<ISurface>(vguiFactory, "VGUI_Surface031");

        g_Panel = get_interface<IPanel>(vgui2Factory, "VGUI_Panel009");

        g_ClientMode = **(IClientMode***)((*(uintptr_t**)g_CHLClient)[10] + 0x5);

        g_Engine = get_interface<IVEngineClient>(engineFactory, "VEngineClient014");

        g_WeaponSystem = *reinterpret_cast<IWeaponSystem**>(Utils::PatternScan("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);

        g_Globals = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_CHLClient))[11] + 10);
    	
        g_Input = *reinterpret_cast<IInput**>(Utils::PatternScan("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);

        Utils::ConsolePrint("{INTERFACES} Success - Interfaces Init", 3);
	}

	void Dump()
	{
    	
	}
}