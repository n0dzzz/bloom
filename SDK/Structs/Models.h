#pragma once
#include "../Math/Vector.h"

struct Model
{
	void* handle;
	char	name[260];
	int	load_flags;
	int	server_count;
	int	type;
	int	flags;
	Vector	vec_mins;
	Vector	vec_maxs;
	float	radius;
};

struct ModelRenderInfo
{
	Vector		origin;
	Vector		angles;
	char		u0[0x4];
	void* renderable;
	const Model* model;
	const Matrix* model_to_world;
	const Matrix* lighting_offset;
	const Vector* lighting_origin;
	int		flags;
	int		entity_index;
	int		skin;
	int		body;
	int		hitboxset;
	unsigned short	instance;

	ModelRenderInfo()
	{
		model_to_world = nullptr;
		lighting_offset = nullptr;
		lighting_origin = nullptr;
	}
};