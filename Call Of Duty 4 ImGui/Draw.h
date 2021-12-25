#pragma once
#include "ImGui/imgui.h"

#include "Vector.h"
#include "WorldToScreen.h"

class Draw {
public:
	static void Draw3DBox(Vec3 HeadPos, ImColor color, int Thickness);
	static void DrawName(ImVec2 Pos, ImColor color, const char* Text);
};
