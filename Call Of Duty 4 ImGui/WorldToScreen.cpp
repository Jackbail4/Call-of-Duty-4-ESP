#include "WorldToScreen.h"

bool w2s::WorldToScreen(Vec3 dst, ImVec2& ScreenPos){
	Vec3x3 matrix;
	Vec3 vLocal, vTransForm;
	AngleVectors(LocalPlayer->ViewAngles, matrix);
	vLocal = SubVectorDist(dst, RefDef->ViewOrig);

	vTransForm.x = vLocal.dotproduct(matrix.vRight);
	vTransForm.y = vLocal.dotproduct(matrix.vUpward);
	vTransForm.z = vLocal.dotproduct(matrix.vForward);

	if (vTransForm.z < 1.f) {
		return false;
	}

	ScreenPos.x = RefDef->ScreenX  / 2 + (RefDef->ScreenX / 2 / vTransForm.z * (1 / RefDef->Fov.x)) * vTransForm.x;
	ScreenPos.y = RefDef->ScreenY / 2 - (RefDef->ScreenY / 2 / vTransForm.z * (1 / RefDef->Fov.y)) * vTransForm.y;
	return true;
}

Vec3 w2s::SubVectorDist(Vec3& playerFrom, Vec3& playerTo)
{
	return Vec3(playerFrom.x - playerTo.x, playerFrom.y - playerTo.y, playerFrom.z - playerTo.z);
}

float w2s::Get3dDistance(Vec3 to, Vec3 from)
{
	return (float)(sqrt(
		((to.x - from.x) * (to.x - from.x)) +
		((to.y - from.y) * (to.y - from.y)) +
		((to.z - from.z) * (to.z - from.z))
	));
}

void w2s::AngleVectors(Vec3 viewAngles, Vec3x3& matrix)
{
	float angle;
	float sr, sp, sy, cr, cp, cy,
		cpi = (3.141f * 2 / 360);

	angle = viewAngles.y * cpi;

	sy = (float)sin(angle);
	cy = (float)cos(angle);
	angle = viewAngles.x * cpi;
	sp = (float)sin(angle);
	cp = (float)cos(angle);
	angle = viewAngles.z * cpi;
	sr = (float)sin(angle);
	cr = (float)cos(angle);

	matrix.vForward.x = cp * cy;
	matrix.vForward.y = cp * sy;
	matrix.vForward.z = -sp;

	matrix.vRight.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	matrix.vRight.y = (-1 * sr * sp * sy + -1 * cr * cy);
	matrix.vRight.z = -1 * sr * cp;

	matrix.vUpward.x = (cr * sp * cy + -sr * -sy);
	matrix.vUpward.y = (cr * sp * sy + -sr * cy);
	matrix.vUpward.z = cr * cp;
}