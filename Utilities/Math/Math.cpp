#include "Math.h"

#include "../Utils.h"
#include "../../SDK/SDK.h"

constexpr double M_PI = 3.14159265358979323846;
constexpr float M_RADPI = 57.295779513082f;
constexpr float M_PI_F = static_cast<float>(M_PI);

constexpr float DEG2RAD(const float x)
{
	return (float)(x) * (float)(M_PI_F / 180.f);
}

namespace Math
{
	void CorrectMove(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove)
	{
		float delta_view;
		float f1;
		float f2;

		if (old_angles.y < 0.f)
			f1 = 360.0f + old_angles.y;
		else
			f1 = old_angles.y;

		if (cmd->viewangles.y < 0.0f)
			f2 = 360.0f + cmd->viewangles.y;
		else
			f2 = cmd->viewangles.y;

		if (f2 < f1)
			delta_view = abs(f2 - f1);
		else
			delta_view = 360.0f - abs(f1 - f2);

		delta_view = 360.0f - delta_view;

		cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
		cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	}

	Vector Math::CalcAimAngle(const Vector& Source, const Vector& Destination, const Vector& ViewAngles)
	{
		Vector Delta = Source - Destination;
		Vector Angles;

		auto RadiansToDegrees = [](float radians) { return radians * 180 / static_cast<float>(M_PI); };
		
		Angles.x = RadiansToDegrees(atanf(Delta.z / std::hypotf(Delta.x, Delta.y))) - ViewAngles.x;
		Angles.y = RadiansToDegrees(atanf(Delta.y / Delta.x)) - ViewAngles.y;
		Angles.z = 0.0f;

		if (Delta.x >= 0.0)
			Angles.y += 180.0f;

		Angles.Normalized();
		return Angles;
	}
	
	Vector CalcAngle(const Vector& a, const Vector& b)
	{
		Vector angles;
		Vector delta;
		delta.x = (a.x - b.x);
		delta.y = (a.y - b.y);
		delta.z = (a.z - b.z);

		double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
		angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
		angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);

		angles.z = 0.0f;
		if (delta.x >= 0.0) { angles.y += 180.0f; }
		return angles;
	}

	QAngle CalcQAngle(const QAngle& a, const QAngle& b)
	{
		QAngle angles;
		QAngle delta;
		delta.pitch = (a.pitch - b.pitch);
		delta.yaw = (a.yaw - b.yaw);
		delta.roll = (a.roll - b.roll);

		double hyp = sqrt(delta.pitch * delta.pitch + delta.yaw * delta.yaw);
		angles.pitch = (float)(atanf(delta.yaw / hyp) * 57.295779513082f);
		angles.yaw = (float)(atanf(delta.yaw / delta.pitch) * 57.295779513082f);

		angles.roll = 0.0f;
		if (delta.pitch >= 0.0) { angles.yaw += 180.0f; }
		return angles;
	}

	void SinCos(float r, float* s, float* c)
	{
		*s = sin(r);
		*c = cos(r);
	}

	Vector AngleVector(Vector angle)
	{
		auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
		auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

		auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
		auto cp = cos(angle.x / 180.f * static_cast<float>(M_PI));

		return Vector(cp * cy, cp * sy, -sp);
	}

	void TransformVectors(Vector& a, Matrix& b, Vector& out)
	{
		out.x = a.Dot(b.mat_val[0]) + b.mat_val[0][3];
		out.y = a.Dot(b.mat_val[1]) + b.mat_val[1][3];
		out.z = a.Dot(b.mat_val[2]) + b.mat_val[2][3];
	}

	void VecAngles(Vector& forward, Vector& angles)
	{
		if (forward.y == 0.0f && forward.x == 0.0f)
		{
			angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
			angles.y = 0.0f;
		}
		else
		{
			angles.x = atan2(-forward.z, Vector2D(forward).length()) * -180 / static_cast<float>(M_PI);
			angles.y = atan2(forward.y, forward.x) * 180 / static_cast<float>(M_PI);

			if (angles.y > 90)
				angles.y -= 180;
			else if (angles.y < 90)
				angles.y += 180;
			else if (angles.y == 90)
				angles.y = 0;
		}

		angles.z = 0.0f;
	}

	void AngleVectors(Vector& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sp, sy, sr, cp, cy, cr;

		SinCos(DEG2RAD(angles.x), &sp, &cp);
		SinCos(DEG2RAD(angles.y), &sy, &cy);
		SinCos(DEG2RAD(angles.z), &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = -1 * sr * sp * cy + -1 * cr * -sy;
			right->y = -1 * sr * sp * sy + -1 * cr * cy;
			right->z = -1 * sr * cp;
		}

		if (up)
		{
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}

	bool ScreenTransform(const Vector& point, Vector& screen)
	{
		auto matrix = Interfaces::g_Engine->WorldToScreenMatrix();

		float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
		screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
		screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
		screen.z = 0.0f;

		int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
			1.0f / w);

		screen.x *= inverse_width;
		screen.y *= inverse_width;
		return (w < 0.001f);
	}

	bool WorldToScreen(const Vector& origin, Vector& screen)
	{
		static std::uintptr_t view_matrix;
		if (!view_matrix)
			view_matrix = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(Utils::PatternScan("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9")) + 3) + 176;

		const auto& matrix = *reinterpret_cast<VMatrix*>(view_matrix);

		const auto w = matrix.m[3][0] * origin.x + matrix.m[3][1] * origin.y + matrix.m[3][2] * origin.z + matrix.m[3][3];
		if (w < 0.001f)
			return false;

		int x, y;
		Interfaces::g_Engine->GetScreenSize(x, y);

		screen.x = static_cast<float>(x) / 2.0f;
		screen.y = static_cast<float>(y) / 2.0f;

		screen.x *= 1.0f + (matrix.m[0][0] * origin.x + matrix.m[0][1] * origin.y + matrix.m[0][2] * origin.z + matrix.m[0][3]) / w;
		screen.y *= 1.0f - (matrix.m[1][0] * origin.x + matrix.m[1][1] * origin.y + matrix.m[1][2] * origin.z + matrix.m[1][3]) / w;

		return true;
	}

	Vector calculate_angle(const Vector& source, const Vector& destination, const Vector& viewAngles)
	{
		Vector delta = source - destination;
		auto radians_to_degrees = [](float radians) { return radians * 180 / static_cast<float>(M_PI); };
		Vector angles;
		angles.x = radians_to_degrees(atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewAngles.x;
		angles.y = radians_to_degrees(atanf(delta.y / delta.x)) - viewAngles.y;
		angles.z = 0.0f;

		if (delta.x >= 0.0)
			angles.y += 180.0f;

		angles.Normalized();
		return angles;
	}

}
