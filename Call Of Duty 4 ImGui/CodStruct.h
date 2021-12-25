#pragma once
#include "Vector.h"
#include <cstdint>

//0x793698
class LocalPlayerInfo
{
public:
	Vec3 ViewAngles; //0x0000
	Vec3 HeadPos; //0x000C
};

//0x74634C
class ClientGameInfo
{
public:
	int32_t ClientNum; //0x0000
	char pad_0004[20]; //0x0004
	int32_t MaxEntities; //0x0018
	char pad_001C[32]; //0x001C
	Vec3 Position; //0x003C
	char pad_0048[288]; //0x0048
	int32_t Health; //0x0168
};

//0x831270 increment-> 0x4CC
class Ent
{
public:
	int32_t Valid; //0x0000
	char pad_0004[8]; //0x0004
	char Name[16]; //0x000C
	int32_t Team; //0x001C
	char pad_0020[888]; //0x0020
	Vec3 Position; //0x0398
};

//0x78F608
class Refdef_t
{
public:
	int32_t ScreenX; //0x0098
	int32_t ScreenY; //0x009C
	Vec2 Fov; //0x00A0
	Vec3 ViewOrig; //0x00A8
};

static LocalPlayerInfo* LocalPlayer = (LocalPlayerInfo*)0x793698;
static ClientGameInfo* CgInfo = (ClientGameInfo*)0x74634C;
static Refdef_t* RefDef = (Refdef_t*)0x78F608;