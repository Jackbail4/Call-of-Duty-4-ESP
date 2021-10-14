#include "Draw.h"

void Draw::DrawLine(ImVec2 Pos, ImColor Color, int Thickness){
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ ImGui::GetIO().DisplaySize.x / 2,ImGui::GetIO().DisplaySize.y }, Pos, Color, Thickness);
}

void Draw::DrawBox(ImVec2 Head, ImVec2 Foot, ImColor Color, int Thickness){
	int Height = Foot.y - Head.y;
	Height /= 4;
	ImVec2 tl, br;
	tl = ImVec2{ Head.x - Height,Head.y };
	br = ImVec2{ Foot.x + Height,Foot.y };
	ImGui::GetBackgroundDrawList()->AddRect(tl, br, Color,0,0,Thickness);
}

void Draw::DrawName(ImVec2 Pos, ImColor Color, char Name[12]){
	ImGui::GetBackgroundDrawList()->AddText(ImVec2{ Pos.x, Pos.y - 15 }, Color, Name);
}
