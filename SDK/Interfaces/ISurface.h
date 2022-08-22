#pragma once

#include "IAppSystem.h"
#include "../Math/Vector2D.h"
#include "../Misc/Color.h"

namespace vgui
{
    typedef unsigned long HFont;
    typedef unsigned int VPANEL;
};

enum FontFeature
{
    FONT_FEATURE_ANTIALIASED_FONTS = 1,
    FONT_FEATURE_DROPSHADOW_FONTS = 2,
    FONT_FEATURE_OUTLINE_FONTS = 6,
};

enum FontDrawType
{
    FONT_DRAW_DEFAULT = 0,
    FONT_DRAW_NONADDITIVE,
    FONT_DRAW_ADDITIVE,
    FONT_DRAW_TYPE_COUNT = 2,
};

enum FontFlags
{
    FONTFLAG_NONE,
    FONTFLAG_ITALIC = 0x001,
    FONTFLAG_UNDERLINE = 0x002,
    FONTFLAG_STRIKEOUT = 0x004,
    FONTFLAG_SYMBOL = 0x008,
    FONTFLAG_ANTIALIAS = 0x010,
    FONTFLAG_GAUSSIANBLUR = 0x020,
    FONTFLAG_ROTARY = 0x040,
    FONTFLAG_DROPSHADOW = 0x080,
    FONTFLAG_ADDITIVE = 0x100,
    FONTFLAG_OUTLINE = 0x200,
    FONTFLAG_CUSTOM = 0x400,
    FONTFLAG_BITMAP = 0x800,
};

struct IntRect
{
    int x0;
    int y0;
    int x1;
    int y1;
};

struct Vertex_t
{
    Vertex_t() {}
    Vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
    {
        m_Position = pos;
        m_TexCoord = coord;
    }
    void Init(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
    {
        m_Position = pos;
        m_TexCoord = coord;
    }

    Vector2D m_Position;
    Vector2D m_TexCoord;
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class ISurface
{
public:
	void SetDrawColor(int r, int g, int b, int a)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[15](this, r, g, b, a);
	}

	void PlaySurfaceSound(const char* sound_path)
	{
		using original_fn = void(__thiscall*)(ISurface*, const char*);
		return (*(original_fn**)this)[82](this, sound_path);
	}

	void SetTextColor(int r, int g, int b, int a)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[25](this, r, g, b, a);
	}

	void DrawPolygon(int n, Vertex_t* vertice, bool clip_vertices = true)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, Vertex_t*, bool);
		return (*(original_fn**)this)[106](this, n, vertice, clip_vertices);
	}
	void DrawFilledRect(int x, int y, int w, int h)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[16](this, x, y, x + w, y + h);
	}
	void SetTexture(int id)
	{
		using original_fn = void(__thiscall*)(ISurface*, int);
		return (*(original_fn**)this)[38](this, id);
	}
	inline void DrawTexturedRect(int x, int y, int w, int h)
	{
		typedef void(__thiscall* original_fn)(void*, int, int, int, int);
		return (*(original_fn**)this)[41](this, x, y, w, h);
	}
	void SetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, const unsigned char*, int, int);
		return (*(original_fn**)this)[37](this, id, rgba, wide, tall);
	}
	int CreateNewTextureID(bool procedural = false)
	{
		using original_fn = int(__thiscall*)(ISurface*, bool);
		return (*(original_fn**)this)[43](this, procedural);
	}
	void DrawRect(int x, int y, int w, int h)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[18](this, x, y, x + w, y + h);
	}
	void DrawLine(int x1, int y1, int x2, int y2)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[19](this, x1, y1, x2, y2);
	}
	void DrawTextFont(unsigned long font)
	{
		using original_fn = void(__thiscall*)(ISurface*, unsigned long);
		return (*(original_fn**)this)[23](this, font);
	}
	void DrawTextPos(int x, int y)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int);
		return (*(original_fn**)this)[26](this, x, y);
	}
	void DrawRenderText(const wchar_t* text, int textLen)
	{
		using original_fn = void(__thiscall*)(ISurface*, const wchar_t*, int, int);
		return (*(original_fn**)this)[28](this, text, textLen, 0);
	}
	unsigned long FontCreate()
	{
		using original_fn = unsigned int(__thiscall*)(ISurface*);
		return (*(original_fn**)this)[71](this);
	}
	void SetFontGlyph(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		using original_fn = void(__thiscall*)(ISurface*, unsigned long, const char*, int, int, int, int, int, int, int);
		return (*(original_fn**)this)[72](this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		using original_fn = void(__thiscall*)(ISurface*, unsigned long, const wchar_t*, int&, int&);
		return (*(original_fn**)this)[79](this, font, text, wide, tall);
	}
	void UnlockCursor()
	{
		using original_fn = void(__thiscall*)(ISurface*);
		return (*(original_fn**)this)[66](this);
	}

	void SetClipRect(int x, int y, int w, int h)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[147](this, x, y, w, h);
	}

	void DrawFilledRectFade(int x, int y, int w, int h, unsigned int alpha1, unsigned int alpha2, bool is_horizontal)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int, unsigned int, unsigned int, bool);
		return (*(original_fn**)this)[123](this, x, y, w, h, alpha1, alpha2, is_horizontal);
	}

	void DrawColoredText(unsigned long font, int x, int y, int red, int green, int blue, int alpha, const char* text)
	{
		using original_fn = void(__thiscall*)(ISurface*, unsigned long, int, int, int, int, int, int, const char*);
		return (*(original_fn**)this)[163](this, font, x, y, red, green, blue, alpha, text);
	}

	void GetScreenSize(int& width, int& height)
	{
		using original_fn = void(__thiscall*)(ISurface*, int&, int&);
		return (*(original_fn**)this)[44](this, std::ref(width), std::ref(height));  //width, height
	}

	void DrawTexturedPolygon(int vertex_count, Vertex_t* vertex, bool clip_vertices = true)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, Vertex_t*, bool);
		return (*(original_fn**)this)[106](this, vertex_count, vertex, clip_vertices);
	}

	void DrawOutlinedCircle(int x, int y, int radius, int segments)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[103](this, x, y, radius, segments);
	}

	void DrawSetTextureFile(int texture_id, const char* texture_name, int hardware_filter, bool force_reload = 0)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, const char*, int, bool);
		return (*(original_fn**)this)[36](this, texture_id, texture_name, hardware_filter, force_reload);
	}

	bool IsTextureIDValid(int texture_id)
	{
		using original_fn = bool(__thiscall*)(ISurface*, int);
		return (*(original_fn**)this)[42](this, texture_id);
	}

	void SurfaceGetCursorPos(int& x, int& y)
	{
		using original_fn = void(__thiscall*)(ISurface*, int&, int&);
		return (*(original_fn**)this)[100](this, std::ref(x), std::ref(y));  //x, y
	}

	void DrawTexturedRectangle(int x, int y, int width, int height)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[41](this, x, y, width, height);
	}
};