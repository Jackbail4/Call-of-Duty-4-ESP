#include "Draw.h"

void Draw::Draw3DBox(Vec3 Pos, ImColor color, int Thickness){
	Vec3 tfl = Vec3{ Pos.x + 10, Pos.y - 10, Pos.z + 55};
	Vec3 tfr = Vec3{ Pos.x + 10, Pos.y + 10, Pos.z + 55 };
	Vec3 tbl = Vec3{ Pos.x - 10, Pos.y + 10, Pos.z + 55 };
	Vec3 tbr = Vec3{ Pos.x - 10, Pos.y - 10, Pos.z + 55 };

	ImVec2 tfl2D, tfr2D, tbl2D, tbr2D;
	w2s::WorldToScreen(tfl, tfl2D);
	w2s::WorldToScreen(tfr, tfr2D);
	w2s::WorldToScreen(tbl, tbl2D);
	w2s::WorldToScreen(tbr, tbr2D);

	Vec3 bfl = Vec3{ Pos.x + 10, Pos.y - 10, Pos.z };
	Vec3 bfr = Vec3{ Pos.x + 10, Pos.y + 10, Pos.z };
	Vec3 bbl = Vec3{ Pos.x - 10, Pos.y + 10, Pos.z };
	Vec3 bbr = Vec3{ Pos.x - 10, Pos.y - 10, Pos.z };

	ImVec2 bfl2D, bfr2D, bbl2D, bbr2D;
	w2s::WorldToScreen(bfl, bfl2D);
	w2s::WorldToScreen(bfr, bfr2D);
	w2s::WorldToScreen(bbl, bbl2D);
	w2s::WorldToScreen(bbr, bbr2D);

	ImGui::GetBackgroundDrawList()->AddLine(tbl2D, tfr2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(tfl2D, tfr2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(tfl2D, tbr2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(tbl2D, tbr2D, color, Thickness);

	ImGui::GetBackgroundDrawList()->AddLine(bbl2D, bfr2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(bfr2D, bfl2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(bfl2D, bbr2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(bbr2D, bbl2D, color, Thickness);

	ImGui::GetBackgroundDrawList()->AddLine(bbr2D, tbr2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(bbl2D, tbl2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(bfr2D, tfr2D, color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(bfl2D, tfl2D, color, Thickness);
}
void Draw::DrawName(ImVec2 Pos, ImColor color, const char* Text) {
	ImGui::GetBackgroundDrawList()->AddText(Pos, color, Text);
}