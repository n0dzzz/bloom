#include "Render.h"

#include "../Utils.h"
#include "../../SDK/SDK.h"

unsigned long Render::Fonts::DefaultFont;
unsigned long Render::Fonts::EspFont;

namespace Render
{
	void Init()
	{
		Render::Fonts::DefaultFont = Interfaces::g_Surface->FontCreate();
		Render::Fonts::EspFont = Interfaces::g_Surface->FontCreate();
		
		Interfaces::g_Surface->SetFontGlyph(Render::Fonts::DefaultFont, "Segoe UI Bold", 24, 500, 0, 0, Fonts::fontflag_antialias);

		Interfaces::g_Surface->SetFontGlyph(Render::Fonts::EspFont, "Arial", 13, 1000, 0, 0, Fonts::fontflag_outline);

		
		Utils::ConsolePrint("{RENDER} Success - Render Init", 3);
	}

	void Text(int x, int y, unsigned long Font, bool Center, std::string Text, Color _Color)
	{
		const auto ConvertedText = std::wstring(Text.begin(), Text.end());
		int Width, Height;
		Interfaces::g_Surface->GetTextSize(Font, ConvertedText.c_str(), Width, Height);

		Interfaces::g_Surface->SetTextColor(_Color.r(), _Color.g(), _Color.b(), _Color.a());
		Interfaces::g_Surface->DrawTextFont(Font);

		if (Center)
			Interfaces::g_Surface->DrawTextPos(x - (Width / 2), y);
		else
			Interfaces::g_Surface->DrawTextPos(x, y);

		Interfaces::g_Surface->DrawRenderText(ConvertedText.c_str(), wcslen(ConvertedText.c_str()));
	}

	void Rect(int x, int y, int w, int h, Color _Color)
	{
		Interfaces::g_Surface->SetDrawColor(_Color.r(), _Color.g(), _Color.b(), _Color.a());

		Interfaces::g_Surface->DrawRect(x, y, w, h);
	}

	void FilledRect(int x, int y, int w, int h, Color _Color)
	{
		Interfaces::g_Surface->SetDrawColor(_Color.r(), _Color.g(), _Color.b(), _Color.a());

		Interfaces::g_Surface->DrawFilledRect(x, y, w, h);
	}

	void TexturedRect(int TextureId, int x, int y, int w, int h, Color _Color)
	{
		Interfaces::g_Surface->SetDrawColor(_Color.r(), _Color.g(), _Color.b(), _Color.a());
		Interfaces::g_Surface->SetTexture(TextureId);

		Interfaces::g_Surface->DrawTexturedRect(x, y, w, h);
	}

	void GetScreenSize(int Width, int Height)
	{
		return Interfaces::g_Surface->GetScreenSize(Width, Height);
	}

	Vector2D GetTextSize(unsigned long font, std::string text)
	{
		std::wstring a(text.begin(), text.end());
		const wchar_t* wstr = a.c_str();
		static int Width, Height;

		Interfaces::g_Surface->GetTextSize(font, wstr, Width, Height);
		return { static_cast<float>(Width), static_cast<float>(Height) };
	}

	void DrawLine(int XPos1, int XPos2, int YPos1, int YPos2, Color LineColor)
	{
		Interfaces::g_Surface->SetDrawColor(LineColor.r(), LineColor.g(), LineColor.b(), LineColor.a());
		Interfaces::g_Surface->DrawLine(XPos1, YPos1, XPos2, YPos2);
	}

	void Circle(std::int32_t x, std::int32_t y, std::int32_t Radius, std::int32_t Segments, Color CircleColor)
	{
		float step = 3.14159265358979323846 * 2.0 / Segments;
		for (float a = 0; a < (3.14159265358979323846 * 2.0); a += step)
		{
			float x1 = Radius * cos(a) + x;
			float y1 = Radius * sin(a) + y;
			float x2 = Radius * cos(a + step) + x;
			float y2 = Radius * sin(a + step) + y;
			Interfaces::g_Surface->SetDrawColor(CircleColor.r(), CircleColor.g(), CircleColor.b(), CircleColor.a());
			Interfaces::g_Surface->DrawLine(x1, y1, x2, y2);
		}
	}
}