#pragma once
#include "../Utilities/Config/Config.h"

struct Variables
{
	
	//
	C_ADDVARIABLE(bool, MISC_Bhop, true);
	C_ADDVARIABLE(bool, MISC_RecoilCrosshair, false);
	C_ADDVARIABLE(Color, MISC_CROSSHAIRCOLOR, Color(47, 25, 197, 255));
	C_ADDVARIABLE(bool, MISC_AntiFlash, false);
	C_ADDVARIABLE(float, MISC_FovOverride, 0);
	C_ADDVARIABLE(bool, MISC_Watermark, false);
	C_ADDVARIABLE(bool, MISC_Thirdperson, false);
	C_ADDVARIABLE(float, MISC_ThirdpersonFOV, 100.f);

	//
	C_ADDVARIABLE(bool, ESP_Skeleton, true);
	C_ADDVARIABLE(bool, ESP_SkeletonXQZ, true);
	C_ADDVARIABLE(bool, ESP_Box, true);
	C_ADDVARIABLE(bool, ESP_AimTargetBoxOverride, true);
	C_ADDVARIABLE(bool, ESP_Names, true);
	C_ADDVARIABLE(bool, ESP_HealthBar , true);
	C_ADDVARIABLE(bool, ESP_TeamCheck, true);
	C_ADDVARIABLE(Color, ESP_BoxColor, Color(255, 255, 255, 255));
	C_ADDVARIABLE(Color, ESP_SkeletonColor, Color(255, 255, 255, 255));
	C_ADDVARIABLE(Color, ESP_NamesColor, Color(255, 255, 255, 255));
	C_ADDVARIABLE(Color, ESP_AimTargetBoxOverrideColor, Color(163, 13, 13));
	
	//
	C_ADDVARIABLE(bool, CHAMS_Enabled, true);
	C_ADDVARIABLE(bool, CHAMS_TeamCheck, true);
	C_ADDVARIABLE(int, CHAMS_Type, 1);
	C_ADDVARIABLE(Color, CHAMS_VisibleColor, Color(33, 173, 67, 255));
	C_ADDVARIABLE(Color, CHAMS_XQZColor, Color(67, 173, 33, 255));
	
	//
	C_ADDVARIABLE(bool, FLAGS_WeaponNames, true);
	C_ADDVARIABLE(bool, FLAGS_IsFlashed, true);
	C_ADDVARIABLE(bool, FLAGS_BombCarrier, true);
	C_ADDVARIABLE(bool, FLAGS_Money, true);
	C_ADDVARIABLE(bool, FLAGS_Armor, true);
	C_ADDVARIABLE(bool, FLAGS_DefuseKit, true);
	C_ADDVARIABLE(bool, FLAGS_Defusing, true);
	C_ADDVARIABLE(bool, FLAGS_Invincible, true);
	C_ADDVARIABLE(bool, FLAGS_AimTarget, true);

	//
	C_ADDVARIABLE(bool, AIM_RCS, true);
	C_ADDVARIABLE(bool, AIM_SILENTAIM, true);
	C_ADDVARIABLE(bool, AIM_VISCHECK, true);
	C_ADDVARIABLE(int, AIM_HITBOX_SELECTION, 1);
	C_ADDVARIABLE(bool, AIM_SMARTHITBOX, true);
	C_ADDVARIABLE(bool, AIM_ENABLED, true);
	C_ADDVARIABLE(float, AIM_FOV, 360.0f);
	C_ADDVARIABLE(float, AIM_SMOOTHAMOUNT, 5.0f);
	C_ADDVARIABLE(bool, AA_RUN, true);
}; 

inline Variables Vars;