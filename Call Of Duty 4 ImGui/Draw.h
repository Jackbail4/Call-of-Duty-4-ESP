#pragma once
#include "ImGui/imgui.h"

class Draw
{
public:
	static void DrawLine(ImVec2 Pos, ImColor Color,int Thickness);
	static void DrawBox(ImVec2 Head, ImVec2 Foot, ImColor Color, int Thickness);
	static void DrawName(ImVec2 Pos, ImColor Color, char Name[12]);
};

