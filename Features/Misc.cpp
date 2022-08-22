#include "Misc.h"

#include "Aim.h"
#include "Variables.h"
#include "../Game.h"
#include "../Hooks.h"
#include "../SDK/Interfaces/CUserCmd.h"
#include "../Utilities/Config/Config.h"
#include "../Utilities/Render/Render.h"
#include "Variables.h"

namespace Misc
{
	void BunnyHop(CUserCmd* cmd)
	{
		if (!C::Get<bool>(Vars.MISC_Bhop))
			return;
		
		if (!(Game::LocalPlayer->flags() & FL_ONGROUND))
		{		
			if (Game::LocalPlayer->MoveType() != MOVETYPE_NOCLIP || MOVETYPE_LADDER)
				cmd->buttons &= ~IN_JUMP;
		}	
	}
	
	void SpotifyDisplay()
	{
		std::string SongTitle = "";
		static HWND SpotifyHWND = nullptr;
		static float LastHWNDTime = 0.f;
		
		if ((!SpotifyHWND || SpotifyHWND == INVALID_HANDLE_VALUE) && LastHWNDTime < Interfaces::g_Globals->realtime + 2.f)
		{
			for (HWND hwnd = GetTopWindow(0); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT))
			{

				LastHWNDTime = Interfaces::g_Globals->realtime;

				if (!(IsWindowVisible)(hwnd))
					continue;

				int length = (GetWindowTextLengthW)(hwnd);
				if (length == 0)
					continue;

				WCHAR filename[300];
				DWORD pid;
				(GetWindowThreadProcessId)(hwnd, &pid);

				const auto SpotifyHandle = (OpenProcess)(PROCESS_QUERY_INFORMATION, FALSE, pid);
				(K32GetModuleFileNameExW)(SpotifyHandle, nullptr, filename, 300);

				std::wstring Filename{ filename };

				(CloseHandle)(SpotifyHandle);

				if (Filename.find((L"Spotify.exe")) != std::string::npos)
					SpotifyHWND = hwnd;
			}
		}
		else if (SpotifyHWND && SpotifyHWND != INVALID_HANDLE_VALUE)
		{
			WCHAR title[300];

			if (!(GetWindowTextW)(SpotifyHWND, title, 300))
			{
				SpotifyHWND = nullptr;
			}
			else
			{
				std::wstring _Title{ title };
				std::string Title = " ";
				std::string Song(_Title.begin(), _Title.end());
				Title += Song;
				
				if (_Title.find((L"-")) != std::string::npos)
				{
					if (Title.length() > 35)
					{
						Title = Title.substr(0, 35);
						Title = Title + "...";
					}
					
					if (C::Get<bool>(Vars.MISC_Watermark))
						Render::Text(Hooks::watermarkx - 2, Hooks::watermarky + 5, Render::Fonts::DefaultFont, false, Title.c_str(), Color(33, 255, 111));
					
					SongTitle = Title;
				}
				else if (_Title.find((L"Advertisment")) != std::string::npos)
				{ 					
					SongTitle = "Advertisment";
				}
				else if (_Title.find((L"Spotify Premium")) != std::string::npos)
				{
					SongTitle = "Stopped/Not Playing";
				}
				else if (_Title.find((L"Spotify")) != std::string::npos)
				{
					SongTitle = "Stopped/Not Playing";
				}
				else
				{
					SongTitle = "Advertisment";
				}

			}
		}
	}

	void DebugInfo()
	{
		if (Game::LocalPlayer == nullptr)
			return;

		if (Game::LocalPlayer->ActiveWeapon() == nullptr)
			return;

		
		Render::Text(325, 200, Render::Fonts::EspFont, false, "Viewmodel Fov: " + std::to_string(C::Get<float>(Vars.MISC_FovOverride)), Color(255, 255, 255));

		Render::Text(325, 220, Render::Fonts::EspFont, false, "LBY: " + std::to_string(Game::LocalPlayer->lower_body_yaw()), Color(255, 255, 255));

		Render::Text(325, 240, Render::Fonts::EspFont, false, "Hitchance: " + std::to_string(Aim::Hitchance), Color(255, 255, 255));

		Render::Text(325, 260, Render::Fonts::EspFont, false, "Cham Type: " + std::to_string(C::Get<int>(Vars.CHAMS_Type)), Color(255, 255, 255));

		std::string AimAngleString = "Aim Angle: (" + std::to_string(Aim::AimAngle.x) + ") (" + std::to_string(Aim::AimAngle.y) + ")" + "(" + std::to_string(Aim::AimAngle.z) + ")";

		Render::Text(325, 280, Render::Fonts::EspFont, false, AimAngleString, Color(255, 255, 255));

		Render::Text(325, 300, Render::Fonts::EspFont, false, "Aim Fov: " + std::to_string(C::Get<float>(Vars.AIM_FOV)), Color(255, 255, 255));

		Render::Text(325, 320, Render::Fonts::EspFont, false, "Hitbox Selection: " + std::to_string(C::Get<int>(Vars.AIM_HITBOX_SELECTION)), Color(255, 255, 255));

	}

	void Thirdperson()
	{
		static bool InThirdperson = false;

		if (GetAsyncKeyState(VK_MBUTTON) & 1)
			InThirdperson = !InThirdperson;

		if (Interfaces::g_Input->CameraInThirdPerson = InThirdperson)
			Interfaces::g_Input->CameraOffset.z = 100;

	}

	void PlayerLookingInfo()
	{
		
	}

	void RecoilCrosshair()
	{
		if (!C::Get<bool>(Vars.MISC_RecoilCrosshair))
		{
			Interfaces::g_Engine->ClientCmd("crosshair 1");
			return;
		}

		if (!Interfaces::g_Engine->IsConnected() && Game::LocalPlayer->IsAlive())
			return;

		if (Game::LocalPlayer == nullptr)
			return;

		std::pair<int, int> ScreenSize;
		int x, y; 

		Interfaces::g_Surface->GetScreenSize(ScreenSize.first, ScreenSize.second);
		x = ScreenSize.first / 2;
		y = ScreenSize.second / 2;

		x -= (ScreenSize.first / 90) * Game::LocalPlayer->aim_punch_angle().y;
		y += (ScreenSize.second / 90) * Game::LocalPlayer->aim_punch_angle().x;

		Interfaces::g_Engine->ClientCmd("crosshair 0");
		Render::FilledRect(x, y - 5, 2, 10, C::Get<Color>(Vars.MISC_CROSSHAIRCOLOR));
		Render::FilledRect(x - 4, y - 1, 10, 2, C::Get<Color>(Vars.MISC_CROSSHAIRCOLOR));

	}

}
