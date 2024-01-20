#pragma once
#include <Windows.h>
#include <iostream>
#include "Mem.h"

struct {
	uintptr_t playerOrgiin = 0x90;
	uintptr_t playerName = 0x4B9;
	uintptr_t playerHealth = 0x390;
	uintptr_t isAlive = 0x3A0;
	std::vector<uintptr_t> head = {0x60, 0x260, 0x188, 0x218, 0x50, 0x0, 0x2AC}; // corresponds to addr.bone
	uintptr_t boneMatrix = {0x1008};
	uintptr_t glow = 0x334; //set to 6 sold, 7 flash + entity
	uintptr_t pitch = 0x1EA8;
	uintptr_t yaw = 0x1EAC;
	uintptr_t team = 0x3A4;
	std::vector<unsigned int> entEyePos = {0x1008, 0x26C};
	std::vector<unsigned int> entBodyPos = {0x1008, 0x17C};
	std::vector<unsigned int> titanBone = { };
	
}off;



struct {
	mem m;
	uintptr_t T2 = m.process(L"Titanfall2.exe");
	uintptr_t mT2 = m.module(T2, L"client.dll");
	uintptr_t m2T2 = m.module(T2, L"engine.dll");
	uintptr_t m3T2 = m.module(T2, L"materialsystem_dx11.dll");
	uintptr_t localplayer = mT2 + 0x00B18748;
	uintptr_t entList = mT2 + 0x00DC9E28; //player offsets seem to be 0x20 apart?
	uintptr_t VM = mT2 + 0xB221E0;
	uintptr_t ADS = mT2 + 0x11C1438;
	uintptr_t eyePos = mT2 + 0xBFCA90;

	uintptr_t npsEntList = mT2 + 0x02787A68;

	uintptr_t lclPlayerAddr = m.readmem<uintptr_t>(localplayer);
	uintptr_t pitchAddr = m.findMA(lclPlayerAddr, off.pitch);
	uintptr_t yawAddr = m.findMA(lclPlayerAddr, off.yaw);
	uintptr_t FOVmax = mT2 + 0x90EB3C;
	uintptr_t FOV = m3T2 + 0x1DCDE58;
}addr;

struct {
	bool ToggleMenu = false;
	bool SnapLine = false;
	bool ESP = false;
	bool BoneESP = false;
	bool Chams = false;
	bool FChams = false;
	bool Aimbot = false;
	bool AimbotCirc = true;
	bool AimbotAim = false;
	bool aimHead = false;
	bool aimBody = true;
	bool aimTog = false;
	bool isAlive = false;
	bool NPC_Aimbot = false;
	bool Lines_NPC_ESP = false, Lines_F_NPC_ESP = false, Lines_Pilot_ESP = true, Lines_F_Pilot_ESP = false;
	bool Box_NPC_ESP = false, Box_F_NPC_ESP = false, Box_Pilot_ESP = true, Box_F_Pilot_ESP = false;
	bool Bone_NPC_ESP = false, Bone_F_NPC_ESP = false, Bone_Pilot_ESP = true, Bone_F_Pilot_ESP = false;
	bool radar = false;
	bool Pilot_Aimbot = true, Pilot_ESP = true, F_Pilot_ESP = false;
	float smoothVal = 10.f, aimFOV = 30.f, distance = 500.f;
	
	float c;
}tog;




