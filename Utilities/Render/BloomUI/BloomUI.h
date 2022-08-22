#pragma once
#include "../Render.h"
#include "../../../SDK/Misc/Color.h"

namespace BloomUI
{
	static int MenuAlpha;
	
	enum BloomUITabLocationFlags
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
	};
	
	enum BloomUIFlags
	{
		NO_FLAGS = 1,
		TITLE_BAR = 2,
		BACKGROUND = 4,
	};
	
	inline std::int32_t UIx, UIy, UIw, UIh;
	inline std::int32_t CheckW, CheckH;
	
	inline bool ShouldDrag = false;
	inline bool ShouldMove = false;
	
	inline int ToggleKey = VK_INSERT;
	inline bool IsToggled;

	void SetToggleKey(int Key);
	void SetIsToggledDefault(bool _IsToggledDefault);
	void Toggle();
	void SetCheckboxWidthAndHeight(std::int32_t Width, std::int32_t Height);
	
	void Begin(std::int32_t XPos, std::int32_t YPos, std::int32_t Width, std::int32_t Height, std::string FrameName, bool IsOutlined, int Flags);

	//void Tabs(std::int32_t TabWidth, std::int32_t TabHeight, std::string TabNames[], std::int32_t& Tab, std::int32_t TabCount);

	void Groupbox(std::int32_t XPos, std::int32_t YPos, std::int32_t Width, std::int32_t Height, std::string GroupboxName);
	
	void Tab(std::int32_t XPos, std::int32_t YPos, std::int32_t Width, std::int32_t Height, std::string TabName, std::int32_t& Tab, std::int32_t TabCount);
	
	void MoveMenu(std::int32_t& XPos, std::int32_t& YPos, std::int32_t Width, std::int32_t Height);

	void CheckBox(std::int32_t& XPos, std::int32_t& YPos, std::string CheckBoxLabel , bool& Var);

	void FadeOnOpen();
	
}