#pragma once
#include "../../SDK/Math/Vector.h"
#include "../../SDK/Math/Vector2D.h"
#include "../../SDK/Interfaces/CUserCmd.h"

namespace Math
{
	void CorrectMove(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove);
	Vector CalcAngle(const Vector& a, const Vector& b);
	Vector CalcAimAngle(const Vector& Source, const Vector& Destination, const Vector& ViewAngles);
	void SinCos(float r, float* s, float* c);
	QAngle CalcQAngle(const QAngle& a, const QAngle& b);
	Vector AngleVector(Vector angle);
	void TransformVectors(Vector&, Matrix&, Vector&);
	void VecAngles(Vector&, Vector&);
	void AngleVectors(Vector&, Vector*, Vector*, Vector*);
	bool ScreenTransform(const Vector& point, Vector& screen);
	bool WorldToScreen(const Vector& origin, Vector& screen);
	Vector calculate_angle(const Vector& source, const Vector& destination, const Vector& viewAngles);
}