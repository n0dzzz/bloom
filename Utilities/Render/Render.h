#pragma once
#include <string>
#include <Windows.h>

#include "../../Menu.h"
#include "../../SDK/Math/Vector2D.h"
#include "../../SDK/Misc/Color.h"

namespace Render
{
	
	namespace Fonts
	{
		enum FontFlags
		{
			fontflag_none,
			fontflag_italic = 0x001,
			fontflag_underline = 0x002,
			fontflag_strikeout = 0x004,
			fontflag_symbol = 0x008,
			fontflag_antialias = 0x010,
			fontflag_gaussianblur = 0x020,
			fontflag_rotary = 0x040,
			fontflag_dropshadow = 0x080,
			fontflag_additive = 0x100,
			fontflag_outline = 0x200,
			fontflag_custom = 0x400,
			fontflag_bitmap = 0x800,
		};

		extern unsigned long DefaultFont;
		extern unsigned long EspFont;
	}
	
	void Init();

	void Text(int x, int y, unsigned long Font, bool Center, std::string Text, Color _Color);

	void Rect(int x, int y, int w, int h, Color _Color);
	
	void FilledRect(int x, int y, int w, int h, Color _Color);
	
	void TexturedRect(int TextureId, int x, int y, int w, int h, Color _Color);

	void DrawLine(int XPos1, int XPos2, int YPos1, int YPos2, Color LineColor);
	
	void GetScreenSize(int Width, int Height);

	Vector2D GetTextSize(unsigned long font, std::string text);

	void Circle(std::int32_t x, std::int32_t y, std::int32_t Radius, std::int32_t Segments, Color CircleColor);
}