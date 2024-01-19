#pragma once
#include <Windows.h>
#include <iostream>
#include "Mem.h"

struct {
	bool singleplayer = false;
	bool infJump = false;
	bool addJump = false;
	bool longJump = false;
	bool infAmmo = false;
	bool infHealth = false;
	bool PTF = false;
	bool Fly = false;
	bool w = false; bool a = false; bool s = false; bool d = false;
	bool togFly = false;
	bool noClip = false;
	bool Speed = false;
	bool fastSwap = false;
	bool noSpread = false;
	bool hidePlayer = false;
	bool infThrust = false;
	bool rapidFire = false;
	bool activatePTF = false;
	bool isMoving = false; 
	bool test = false; // testing
	bool S_Aimbot = false; bool entBody; bool entEye; bool S_Aimbot_Tog;
	bool S_Snapline = false;
	bool S_boxESP = false;
	bool S_boneESP = false;
	float jumpMul = 1.5f;
	float flySpeed = 1.f;
	float ptfSpeed = 1.f;
	 float c;
	 bool AimCirc = true; float aimSmooth = 1.f;
	 float Line_color[3] = { 1.f, 1.f, 1.f };
	 float Box_color[3] = { 1.f, 1.f, 1.f };
	 float Bone_color[3] = { 1.f, 1.f, 1.f };
	 float Circ_color[3] = { 1.f, 1.f, 1.f };
	 float radar_Circ_color[3] = { 0.f, 0.f, 0.f }, player_dot_color[3] = {0.f, 1.f, 0.f}, NPC_dot_color[3] = {1.f, 0.f, 0.f};
	 float Border_color[3] = { 1.f, 1.f, 1.f };
	 bool radarFill = false;
	 bool radar2D = false;
	 bool radarRotaed = true, radarNotRotated = false, radarRotationTog = false;
	 float radarRenderDist = 1000.f, radarCirc = 0.20f, radarAlpha = 9.f, NPCSize = 5.f, PilotSize = 5.f, PlayerSize = 5.f, radarBorderAlpha = 9.f;
	 bool radarNPC = true, radarPilot = true, radarNPC_F = false, radarPilot_F = false, radarLocalPlayer = true, radarEdgeDot = true, radarBackground = true;
	 Vec2 radarPos = {-.87f, .7f};
	 bool radarMenuEX = false, radarBorder = false;
}Stog;

struct {
	uintptr_t originOff = 0x490;
	uintptr_t Xoff = 0x490;
	uintptr_t Yoff = 0x494;
	uintptr_t Zoff = 0x498;
	uintptr_t ZVelOff = 0x4B0;
	uintptr_t XVelOff = 0x4A8;
	uintptr_t YVelOff = 0x4AC;
	uintptr_t C_ZVelOff = 0x25C;
	uintptr_t C_XVelOff = 0x254;
	uintptr_t C_YVelOff = 0x258;
	uintptr_t playerHealth = 0x4D4;
	std::vector<unsigned int> entName = {0x4B0, 0x0};
	std::vector<unsigned int> entEye = { 0x1008, 0x26C };
	std::vector<unsigned int> entBody = { 0x1008, 0x17C };
}Soff;

struct {
	mem m;
	uintptr_t T2 = m.process(L"Titanfall2.exe");
	uintptr_t mT2 = m.module(T2, L"client.dll");
	uintptr_t m2T2 = m.module(T2, L"engine.dll");
	uintptr_t m3T2 = m.module(T2, L"materialsystem_dx11.dll");
	uintptr_t m4T2 = m.module(T2, L"server.dll");
	
	uintptr_t localPlayer = m2T2 + 0x1308C248;
	uintptr_t origin = m2T2 + 0xF84BE0C;
	uintptr_t velocity = m4T2 + 0x432B85;
	uintptr_t Xvelocity = m4T2 + 0x432B32;
	uintptr_t Yvelocity = m4T2 + 0x432B5B;
	uintptr_t infAmmo = m4T2 + 0x6A1F30;
	uintptr_t infHealth = m4T2 + 0x43373A;
	uintptr_t fastSwap1 = m4T2 + 0xF38DD;
	uintptr_t fastSwap2 = m4T2 + 0xBD324;
	uintptr_t nospread1 = m4T2 + 0x6AB273;
	uintptr_t nospread2 = m4T2 + 0x69EEAE;
	uintptr_t hidePlayer = mT2 + 0x1A7D97;
	uintptr_t infThrust = m4T2 + 0x172FA6;
	uintptr_t rapidFire1 = m4T2 + 0x30C164;
	uintptr_t rapidFire2 = m4T2 + 0x30C15C;
	
	// -----------------------------TEST AREA---------------------------
	uintptr_t test = m4T2 + 0x30C164; // rapid fire
	uintptr_t test2 = mT2 + 0x14303C; // no view recil
	uintptr_t test3 = m4T2 + 0x30C15C; // rapid fire
	// -----------------------------TEST AREA---------------------------
	// NPC DATA:
	//   EntList: client + 0x02787A68
	//   Name: 0x4B0 (a pointer) -> 0x0 (string)
	//   Health: 0x390 (unk) (max 50?)
	//   Pos: 0x90 (unk)

	// Weapon DATA:
	//   EntList: client + 0x02787A68
	//   Name: 0x40 (pointer) -> 0x0 (string) 
	//   Pos: 0x90 (unk)

}Saddr;

