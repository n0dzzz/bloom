#pragma once
#include "IMaterialSystem.h"
#include <cstddef>
#include <string_view>

enum class override_type
{
    normal = 0,
    build_shadows,
    depth_write,
    custom_material, // weapon skins
    ssao_depth_write
};

class IStudioRender
{
    std::byte pad_0[0x250];
    IMaterial* material_override;
    std::byte pad_1[0xC];
    override_type override_type;
public:
    bool IsForced() noexcept
    {
        if (!material_override)
            return override_type == override_type::depth_write || override_type == override_type::ssao_depth_write;
        return std::string_view{ material_override->GetName() }._Starts_with("dev/glow");
    }
};