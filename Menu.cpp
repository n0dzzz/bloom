#include "Menu.h"

#include <vector>
#include "SDK/SDK.h"
#include <Windows.h>


#include "Dependencies/ImGui/imgui_impl_dx9.h"
#include "Dependencies/ImGui/imgui_impl_win32.h"
#include "Features/Variables.h"
#include "Utilities/Config/Config.h"
#include "Utilities/Render/BloomUI/BloomUI.h"
#include <windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImGuiEx
{
	inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
	{
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		auto clr = ImVec4{
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, &clr.x, show_alpha))
		{
			v->SetColor(clr.x, clr.y, clr.z, clr.w);
			return true;
		}
		return false;
	}
	inline bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v, false);
	}
}
namespace Menu
{	
	/*
	 * void Render()
	{
		if (!BloomUI::IsToggled)
			return;

		BloomUI::FadeOnOpen();
		BloomUI::SetToggleKey(VK_INSERT);
		BloomUI::SetIsToggledDefault(true);

		BloomUI::Begin(x, y, w, h, "storm", true, BloomUI::TITLE_BAR | BloomUI::BACKGROUND);

		Render::FilledRect(x + 100, y + 30, w - 899, h - 30, Color(35, 47, 235, 255));
		Render::FilledRect(x + 100, y + 95, w - 100, h - 549, Color(35, 47, 235, 255));

		BloomUI::Tab(x, y + 30, 100, 100, "Aim", TabCount, 0);
		BloomUI::Tab(x, y + 130, 100, 100, "Visual", TabCount, 1);
		BloomUI::Tab(x, y + 230, 100, 100, "Misc", TabCount, 2);

		switch (TabCount)
		{
		case 0:

			BloomUI::Groupbox(x + 155, y + 110, 300, 300, "General");

			BloomUI::Groupbox(x + 545, y + 110, 300, 300, "Accuracy");

			break;

		case 1:

			BloomUI::Groupbox(x + 155, y + 110, 300, 300, "Player");

			BloomUI::Groupbox(x + 545, y + 110, 300, 300, "World");

			break;

		case 2:
			BloomUI::Groupbox(x + 155, y + 110, 300, 300, "General");

			BloomUI::Groupbox(x + 545, y + 110, 300, 300, "Other");

			break;
		}

		BloomUI::MoveMenu(x, y, w, h);
	}
	 */
}

// Ready to go if you like imgui i fucking hate it personally
namespace ImGuiMenu
{
	void Space()
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
	}
	
	float FadeOpen()
	{
		static float flMenuAlpha = 0.0f;
		float MenuAlpha = flMenuAlpha * 2.55f;

		if (!IsVisible)
		{
			flMenuAlpha -= (0.1f / 0.02f) * Interfaces::g_Globals->frametime * 1.2;
			float MenuAlphaA = flMenuAlpha;
			MenuAlpha = flMenuAlpha * 255;

			if (flMenuAlpha <= 0.01f)
			{
				flMenuAlpha = 0.0f;
				return 0;
			}
		}
		else
		{
			flMenuAlpha += (0.10f / 0.02f) * Interfaces::g_Globals->frametime * 1.2;
			MenuAlpha = flMenuAlpha * 255;

			if (flMenuAlpha >= 0.9f)
				flMenuAlpha = 1.0f;
		}

		return flMenuAlpha;
	}

	ImGuiStyle MyStyle;

	void CustomStyle()
	{
		ImGui::StyleColorsDark();
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);
		MyStyle.FrameRounding = 1.f;
		MyStyle.WindowRounding = 1.f;
		MyStyle.ChildRounding = 0.f;
		MyStyle.Colors[ImGuiCol_Button] = ImVec4(0.260f, 0.590f, 0.980f, 0.670f);
		MyStyle.Colors[ImGuiCol_Header] = ImVec4(0.260f, 0.590f, 0.980f, 0.670f);
		MyStyle.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
		//_style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.000f, 0.545f, 1.000f, 1.000f);
		//_style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.060f, 0.416f, 0.980f, 1.000f);
		MyStyle.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.30f, 1.00f);
		MyStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.030f, 0.033f, 0.033f, 0.65f);
		MyStyle.Colors[ImGuiCol_PopupBg] = ImVec4(0.076f, 0.143f, 0.209f, 1.00f);
		MyStyle.Colors[ImGuiCol_Separator] = ImVec4(0.260f, 0.590f, 0.980f, 0.670f);

		ImGui::GetStyle() = MyStyle;
	}

	void Toggle()
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			ImGui::SetNextWindowBgAlpha(FadeOpen());
			IsVisible = !IsVisible;
		}
	}

	void Render()
	{
		CustomStyle();
		ImGui::SetNextWindowPos(ImVec2{ 100, 100 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 900, 600 }, ImGuiCond_Once);

		C::Refresh();

		if (ImGui::Begin("bloom", &IsVisible, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			const ImVec2 VecPosition = ImGui::GetCursorScreenPos();
			const float WindowWidth = ImGui::GetWindowWidth();
			const float WindowHeight = ImGui::GetWindowHeight();
			ImDrawList* WindowDrawList = ImGui::GetWindowDrawList();

			ImGui::Text("bloom");
			ImGui::Separator();

			ImGui::Columns(3);
			
			ImGui::SetColumnOffset(1, 300);
			{
				ImGui::Text("-- Aim");
				Space();

				ImGui::Checkbox("Aim Run", &C::Get<bool>(Vars.AIM_ENABLED));
				ImGui::Checkbox("Silent Aim", &C::Get<bool>(Vars.AIM_SILENTAIM));
				ImGui::SliderFloat("Smooth Amount", &C::Get<float>(Vars.AIM_SMOOTHAMOUNT), 0.0f, 5.0f);
				ImGui::SliderFloat("FOV", &C::Get<float>(Vars.AIM_FOV), 0.0f, 360.0f);
				ImGui::Checkbox("Smart Hitbox", &C::Get<bool>(Vars.AIM_SMARTHITBOX));

				const char* HitboxSelections[] = { "Head","Body" };
				ImGui::Combo("Hitbox Selection", &C::Get<int>(Vars.AIM_HITBOX_SELECTION), HitboxSelections, IM_ARRAYSIZE(HitboxSelections), 0.5);

				ImGui::Checkbox("RCS", &C::Get<bool>(Vars.AIM_RCS));

			}
			ImGui::NextColumn();	

			ImGui::SetColumnOffset(2, 600);
			{
				ImGui::Text("-- Visual");
				Space();

				ImGui::Checkbox("Team Check", &C::Get<bool>(Vars.ESP_TeamCheck));
				ImGui::Checkbox("Boxes", &C::Get<bool>(Vars.ESP_Box));
				ImGui::SameLine();
				ImGuiEx::ColorEdit4("Box Color", &C::Get<Color>(Vars.ESP_BoxColor));
				
				ImGui::Checkbox("Aim Target Box Color Override", &C::Get<bool>(Vars.ESP_AimTargetBoxOverride));
				ImGui::SameLine();
				ImGuiEx::ColorEdit4("Box Color Override", &C::Get<Color>(Vars.ESP_AimTargetBoxOverrideColor));

				ImGui::Checkbox("Names", &C::Get<bool>(Vars.ESP_Names));
				ImGui::SameLine();
				ImGuiEx::ColorEdit4("Names Color", &C::Get<Color>(Vars.ESP_NamesColor));
				
				ImGui::Checkbox("Health Bar", &C::Get<bool>(Vars.ESP_HealthBar));
				
				ImGui::Checkbox("Skeleton", &C::Get<bool>(Vars.ESP_Skeleton));
				ImGui::SameLine();
				ImGuiEx::ColorEdit4("Skeleton Color", &C::Get<Color>(Vars.ESP_SkeletonColor));
				ImGui::Checkbox("Skeleton XQZ", &C::Get<bool>(Vars.ESP_SkeletonXQZ));

				ImGui::Checkbox("Antiflash", &C::Get<bool>(Vars.MISC_AntiFlash));

				Space();
				ImGui::Text("-- Flags");
				Space();
				
				ImGui::Checkbox("Weapon Names", &C::Get<bool>(Vars.FLAGS_WeaponNames));
				ImGui::Checkbox("Armor", &C::Get<bool>(Vars.FLAGS_Armor));
				ImGui::Checkbox("Bomb Carrier", &C::Get<bool>(Vars.FLAGS_BombCarrier));
				ImGui::Checkbox("Defuse Kit", &C::Get<bool>(Vars.FLAGS_DefuseKit));
				ImGui::Checkbox("Defusing ", &C::Get<bool>(Vars.FLAGS_Defusing));
				ImGui::Checkbox("Flashed ", &C::Get<bool>(Vars.FLAGS_IsFlashed));
				ImGui::Checkbox("Money", &C::Get<bool>(Vars.FLAGS_Money));
				ImGui::Checkbox("Invincible", &C::Get<bool>(Vars.FLAGS_Invincible));
				ImGui::Checkbox("Aim Target", &C::Get<bool>(Vars.FLAGS_AimTarget));

				ImGui::Spacing();		
				Space();

				Space();
				ImGui::Text("-- Chams");
				Space();

				ImGui::Checkbox("Chams Run", &C::Get<bool>(Vars.CHAMS_Enabled));
				ImGui::Checkbox("Chams Team Check", &C::Get<bool>(Vars.CHAMS_TeamCheck));

				ImGui::Combo("Cham Type", &C::Get<int>(Vars.CHAMS_Type), "Non XQZ\0XQZ\0");

				ImGui::Spacing();
				
				ImGui::Text("Visible Color");
				ImGui::SameLine();
				ImGuiEx::ColorEdit4("Visible Color", &C::Get<Color>(Vars.CHAMS_VisibleColor));

				ImGui::Text("XQZ Color");
				ImGui::SameLine();
				ImGuiEx::ColorEdit4("XQZ Color", &C::Get<Color>(Vars.CHAMS_XQZColor));

				ImGui::Spacing();			
				Space();
				
				Space();
				ImGui::Text("-- Skins");
				Space();


				
				ImGui::Spacing();
				
				Space();
			}
			ImGui::NextColumn();
			

			ImGui::SetColumnOffset(3, 900);
			{
				ImGui::Text("-- Miscellaneous");
				Space();

				ImGui::Checkbox("Bunny Hop", &C::Get<bool>(Vars.MISC_Bhop));
				ImGui::Checkbox("Watermark", &C::Get<bool>(Vars.MISC_Watermark));
				ImGui::Checkbox("Third Person", &C::Get<bool>(Vars.MISC_Thirdperson));

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.135f);
				ImGui::SliderFloat("Third Person FOV", &C::Get<float>(Vars.MISC_ThirdpersonFOV), 0, 200);
				ImGui::PopItemWidth();

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.135f);
				ImGui::SliderFloat("FOV Override", &C::Get<float>(Vars.MISC_FovOverride), 0, 200);
				ImGui::PopItemWidth();

				ImGui::Checkbox("Recoil Crosshair", &C::Get<bool>(Vars.MISC_RecoilCrosshair));

				ImGui::Spacing();

				ImGui::Text("Crosshair Color");
				ImGui::SameLine();
				ImGuiEx::ColorEdit4("Crosshair Color", &C::Get<Color>(Vars.MISC_CROSSHAIRCOLOR));

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("-- Config");
				Space();

				static char Text[] = "New Config";
				std::vector<std::string> Config = {};
				static int ConfigSelection;

				for (auto i = 0; i < C::vecFileNames.size(); i++)
				{
					Config.push_back(C::vecFileNames.at(i));
				}

				int i = 0;
				for (std::vector<std::string>::iterator it = Config.begin(); it != Config.end(); ++it)
				{
					if (ImGui::Selectable(Config.at(i).c_str(), i == ConfigSelection))
					{
						ConfigSelection = i;
					}

					i++;
				}


				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.135f);
				ImGui::InputText("Config Name", Text, IM_ARRAYSIZE(Text));
				ImGui::PopItemWidth();

				Space();

				if (ImGui::Button("Create", ImVec2{ 100, 20 }))
				{
					C::Save(Text);
					C::Refresh();
				}

				ImGui::Spacing();

				if (ImGui::Button("Remove", ImVec2{ 100, 20 }))
				{
					if (Config.empty())
						return;

					C::Remove(Config.at(ConfigSelection));
					C::Refresh();
				}

				ImGui::Spacing();

				if (ImGui::Button("Save", ImVec2{ 100, 20 }))
				{
					if (Config.empty())
						return;

					C::Save(Config.at(ConfigSelection));
				}

				ImGui::Spacing();

				if (ImGui::Button("Load", ImVec2{ 100, 20 }))
				{
					if (Config.empty())
						return;

					C::Load(Config.at(ConfigSelection));
				}

			}
		}
			ImGui::End();

	}

	bool Init(IDirect3DDevice9* Device)
	{
		_HWND = FindWindow("Valve001", "Counter-Strike: Global Offensive - Direct3D 9");
		WNDProcOrigin = reinterpret_cast<WNDPROC>(SetWindowLongPtr(_HWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WinProc)));

		ImGui::CreateContext();  // ImGuiIO& io = ImGui::GetIO(); 
		CustomStyle();
		ImGui_ImplWin32_Init(_HWND);
		ImGui_ImplDX9_Init(Device);

		Initialiazed = true;
		return true;
	}

	void Release()
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		SetWindowLongPtr(_HWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WNDProcOrigin));
	}

	LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		if (IsVisible && ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
			return true;

		return CallWindowProc(WNDProcOrigin, hWnd, Msg, wParam, lParam);
	}

}
