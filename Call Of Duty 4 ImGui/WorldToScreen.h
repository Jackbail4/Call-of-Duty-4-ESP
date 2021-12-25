#pragma once
#include "ImGui/imgui.h"
#include "Vector.h"
#include "WorldToScreen.h"

#include "CodStruct.h"

class w2s {
public:
	static bool WorldToScreen(Vec3 dst, ImVec2& ScreenPos);
	static float Get3dDistance(Vec3 to, Vec3 from);
private:
	static Vec3 SubVectorDist(Vec3& playerFrom, Vec3& playerTo);
	static void AngleVectors(Vec3 viewAngles, Vec3x3& matrix);
};