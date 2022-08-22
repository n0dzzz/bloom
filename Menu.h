#pragma once

#include <d3d9.h>
#include <Windows.h>
//#include "Utilities/Render/BloomUI/BloomUI.h"

namespace Menu
{
	inline int x = 100;
	inline int y = 100;
	inline int w = 900;
	inline int h = 550;
	inline int TabCount{0};
	void Render();
}

 namespace ImGuiMenu
{
	inline bool IsVisible = false;
	inline bool Initialiazed = false;
	inline HWND _HWND;
	inline WNDPROC WNDProcOrigin;

	void Toggle();
	bool Init(IDirect3DDevice9* Device);
	void Render();
	void Release();
	LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
}