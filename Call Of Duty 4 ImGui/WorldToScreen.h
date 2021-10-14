#pragma once
#include "ImGui/imgui.h"
#include "Vector.h"

class w2s {
public:
	static bool WorldToScreen(Vec3 dst, Vec3 src, ImVec2& ScreenPos, Vec3x3 matrix, Vec3 viewAngles, ImVec2 WindowRes, Vec2 Fov);
	static float Get3dDistance(Vec3 to, Vec3 from);
private:
	static Vec3 SubVectorDist(Vec3& playerFrom, Vec3& playerTo);
	static void AngleVectors(Vec3 viewAngles, Vec3x3& matrix);
};