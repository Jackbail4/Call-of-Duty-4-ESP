#pragma once
#include <stdint.h>
#include "Vector.h"

class Ent
{
public:
	int32_t Valid; //0x0000
	char pad_0004[8]; //0x0004
	char Name[16]; //0x000C
	int32_t Team; //0x001C
	char pad_0020[156]; //0x0020
	char WeaponName[12]; //0x00BC
	char pad_00C0[720]; //0x00C0
	Vec3 HeadPos; //0x0398
}; //Size: 0x0444