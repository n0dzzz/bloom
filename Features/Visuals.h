#pragma once
#include "../SDK/SDK.h"
#include "../SDK/Interfaces/CUserCmd.h"
#include "../Utilities/Render/Render.h"
#include "Variables.h"
#include "../Game.h"
#include "../SDK/Classes/Studio.h"
#include "../SDK/Interfaces/IMaterialSystem.h"
#include "../Utilities/Config/Config.h"
#include "Aim.h";

namespace Visuals
{
	void Render();
	void Chams(MatRenderContext* ctx, const DrawModelState& state, const ModelRenderInfo& info, Matrix* matrix);
}
