#include "BloomUI.h"


#include "../../Utils.h"
#include "../../../SDK/SDK.h"

namespace BloomUI
{	
	POINT Cursor;
	POINT CursorCorrected;
	
	Color UIColor = Color(35, 35, 35, 255);
	Color OutlineColor = Color(35, 47, 235, 255);
	Color DimOutlineColor = Color(35, 47, 235, 105);
	Color BackgroundColor = Color(25, 25, 25, 100);
	Color GroupboxFillColor = Color(35, 35, 35, 255);

	void MoveMenu(std::int32_t& XPos, std::int32_t& YPos, std::int32_t Width, std::int32_t Height)
	{
		GetCursorPos(&Cursor);

		if (GetAsyncKeyState(VK_LBUTTON) < 0 && (Cursor.x > XPos && Cursor.x < XPos + Width && Cursor.y > YPos && Cursor.y < YPos + Height))
		{
			ShouldDrag = true;

			if (!ShouldMove)
			{
				CursorCorrected.x = Cursor.x - XPos;
				CursorCorrected.y = Cursor.y - YPos;
				ShouldMove = true;
			}
		}

		if (ShouldDrag)
		{
			XPos = Cursor.x - CursorCorrected.x;
			YPos = Cursor.y - CursorCorrected.y;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0)
		{
			ShouldDrag = false;
			ShouldMove = false;
		}
	}
	
	void Begin(std::int32_t XPos, std::int32_t YPos, std::int32_t Width, std::int32_t Height, std::string FrameName, bool IsOutlined, int Flags)
	{
		UIx = XPos;
		UIy = YPos;
		UIw = Width;
		UIh = Height;

		// Have to render background before frame so it dosent mess up the colors (:
		if (Flags & BACKGROUND)
		{
			int w, h;
			Interfaces::g_Engine->GetScreenSize(w, h);
			Render::FilledRect(0, 0, w, h, BloomUI::BackgroundColor);
		}

		// instead of making the user handle the tedious positioning we can just do it on the backend
		// so we are just saving the inital values into variables we can use
		
		Render::FilledRect(XPos, YPos, Width, Height, BloomUI::UIColor);

		if (IsOutlined)
			Render::Rect(XPos, YPos, Width, Height, BloomUI::OutlineColor);
	
		if (Flags & TITLE_BAR)
		{
			Render::FilledRect(XPos, YPos, Width, Height - Height + 30, BloomUI::UIColor);
			Render::Text(XPos + 15, YPos + 5, Render::Fonts::DefaultFont, false, FrameName, Color(255, 255, 255));

			if (IsOutlined)
				Render::Rect(XPos, YPos, Width, Height - Height + 30, BloomUI::OutlineColor);
		}
		
	}

	void SetToggleKey(int Key)
	{
		ToggleKey = Key;
	}

	void SetIsToggledDefault(bool _IsToggledDefault)
	{
		IsToggled = _IsToggledDefault;
	}
	
	void Toggle()
	{
		if (GetAsyncKeyState(ToggleKey) & 1)
			IsToggled = !IsToggled;
	}
	
	void SetCheckboxWidthAndHeight(std::int32_t Width, std::int32_t Height)
	{
		CheckW = Width;
		CheckH = Height;
	}

	/* Eventually i want to do tabs with auto positioning... but today is not the day
	 void Tab(std::int32_t TabWidth, std::int32_t TabHeight, std::string TabNames[], std::int32_t& Tab, std::int32_t TabCount)
	{
		GetCursorPos(&Cursor);

		int TabY = UIy;
		std::int32_t TextY = UIy + TabHeight / 1.5;
		
		if ((Cursor.x > UIx) && (Cursor.x < UIx + UIw) && (Cursor.y > UIy) && (Cursor.y < UIy + UIh) && (GetAsyncKeyState(VK_LBUTTON) & 1))
			Tab = TabCount;

		for (int i = 0; i < sizeof(TabNames); i++)
		{	
			Render::Rect(UIx, TabY + 31, TabWidth, TabHeight, OutlineColor);
			Render::Text(UIx + TabWidth / 2, TextY, Render::Fonts::DefaultFont, true, TabNames[i], Color(255, 255, 255));

			// I would love to figure out some math problem that would be able to size the tabs without parameters too lazy to at the moment
			
			TabY += TabHeight;
			TextY += TabHeight;
		}	
	}
	 */

	void Tab(std::int32_t XPos, std::int32_t YPos, std::int32_t Width, std::int32_t Height, std::string TabName, std::int32_t& Tab, std::int32_t TabCount)
	{
		GetCursorPos(&Cursor);
		
		if ((Cursor.x > XPos) && (Cursor.x < XPos + Width) && (Cursor.y > YPos) && (Cursor.y < YPos + Height) && (GetAsyncKeyState(VK_LBUTTON) & 1))
			Tab = TabCount;

		Render::Rect(XPos, YPos, Width, Height, Tab == TabCount ? BloomUI::OutlineColor : BloomUI::DimOutlineColor);

		Render::Text(XPos - Render::GetTextSize(Render::Fonts::DefaultFont, TabName).x / 2 + Width / 2, YPos + Height / 2 - 8, Render::Fonts::DefaultFont, false, TabName, Color(255, 255, 255));
	}
	
	void CheckBox(std::int32_t& XPos, std::int32_t& YPos, std::string CheckBoxLabel, bool& Var)
	{
		GetCursorPos(&Cursor);

		if ((Cursor.x > XPos) && (Cursor.x < XPos + CheckW) && (Cursor.y > YPos) && (Cursor.y < YPos + CheckH) && GetAsyncKeyState(VK_LBUTTON) & 1)
			Var = !Var;

		Render::Rect(XPos, YPos, CheckW, CheckH, Var ? BloomUI::OutlineColor : BloomUI::DimOutlineColor);

		Render::Text(XPos - 3, YPos, Render::Fonts::DefaultFont, false, CheckBoxLabel, Color(255,255,255));
		
	}

	void FadeOnOpen()
	{
		static float flMenuAlpha = 0.0f;
		MenuAlpha = flMenuAlpha * 255.0f;
		
		if (!IsToggled)
		{
			flMenuAlpha -= (1.0f / 0.2f) * Interfaces::g_Globals->frametime * 1.2;
			float MenuAlphaA = flMenuAlpha;
			MenuAlpha = flMenuAlpha * 255;
			if (flMenuAlpha <= 0.01f)
			{
				flMenuAlpha = 0.0f;
				return;
			}
		}
		else
		{
			flMenuAlpha += (1.0f / 0.2f) * Interfaces::g_Globals->frametime * 1.2;
			MenuAlpha = flMenuAlpha * 255;
			if (flMenuAlpha >= 0.9f)
				flMenuAlpha = 1.0f;
			
		}
	}

	void Groupbox(std::int32_t XPos, std::int32_t YPos, std::int32_t Width, std::int32_t Height, std::string GroupboxName)
	{
		Render::FilledRect(XPos, YPos, Width, Height, GroupboxFillColor);
		Render::Rect(XPos, YPos, Width, Height, OutlineColor);
		Render::Text(XPos + 150, YPos - 12, Render::Fonts::DefaultFont, true, GroupboxName, Color(255, 255, 255));
	}
	
}
