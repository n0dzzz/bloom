#include <chrono>
#include <thread>
#include <Windows.h>



#include "Hooks.h"
#include "Menu.h"
#include "SDK/SDK.h"
#include "Utilities/Options.h"
#include "Utilities/Utils.h"
#include "Utilities/Config/Config.h"
#include "Utilities/Render/Render.h"
#include <windows.h>

unsigned long WINAPI Init(void* instance)
{
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	Utils::AttachConsole("Wiiner");
#endif

	try
	{
		Interfaces::Init();
		Hooks::Init();
		Render::Init();

		C::Setup("default.cfg");
		
	} catch (const std::runtime_error& error)
	{
		MessageBoxA(nullptr, error.what(), "GBase Error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	
}

unsigned long WINAPI Detach()
{
#ifdef _DEBUG
	Utils::DetachConsole();
#endif

	Hooks::Detatch();
	
	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] )
{
	DisableThreadLibraryCalls(instance);

	switch (reason)
	{
	case DLL_PROCESS_ATTACH: {
			
		if (auto handle = CreateThread(nullptr, NULL, Init, instance, NULL, nullptr))
		{
			CloseHandle(handle);
		}
			
		break;
	}
	case DLL_PROCESS_DETACH: {
			
		Detach();
			
		break;
	}
	}

	return true;
}