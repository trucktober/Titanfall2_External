#pragma once
#include "T2.h"
#include "Vector Structor.h"
#include <GLFW/glfw3.h> 
#include <math.h>
#define PI 3.14159265359

bool WTS(const Vec3& Origin, Vec2& Pos, float* Matrix) {
	Vec2 Res{ Res.x = GetSystemMetrics(SM_CXSCREEN) / 2, Res.y = GetSystemMetrics(SM_CYSCREEN) / 2 };

	Pos.x = (Origin.x * Matrix[0]) + (Origin.y * Matrix[1]) + (Origin.z * Matrix[2]) + Matrix[3];
	Pos.y = (Origin.x * Matrix[4]) + (Origin.y * Matrix[5]) + (Origin.z * Matrix[6]) + Matrix[7];
	/*Pos.z = (Origin.x * Matrix[8]) + (Origin.y * Matrix[9]) + (Origin.z * Matrix[10]) + Matrix[11];*/
	float w = (Origin.x * Matrix[12]) + (Origin.y * Matrix[13]) + (Origin.z * Matrix[14]) + Matrix[15];

	if (w < 0.01f) {
		return false;
	}


	Vec2 NDC;
	NDC.x = Pos.x / w;
	NDC.y = Pos.y / w;

	Pos.x = (((Res.x * NDC.x) + (NDC.x + Res.x)) / (Res.x * 2.f)) * 2.f - 1.f;
	Pos.y = (((Res.y * NDC.y) + (NDC.y + Res.y)) / (Res.y * 2.f)) * 2.f - 0.978f;

	return true;
}

uintptr_t DrawLines(Vec2 &Pos) {
	glColor3f(Stog.Line_color[0], Stog.Line_color[1], Stog.Line_color[2]);
	glLineWidth(1.f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(snap.start.x, snap.start.y);
    glVertex2f(Pos.x, Pos.y);
	glEnd();
    return true;

}

uintptr_t DrawCircle(float& circ, Vec2& pos) {
	glColor3f(Stog.Circ_color[0], Stog.Circ_color[1], Stog.Circ_color[2]);
	glLineWidth(1.f);
	Vec2 Res{ Res.x = GetSystemMetrics(SM_CXSCREEN), Res.y = GetSystemMetrics(SM_CYSCREEN) - 60.f };
	float ResScale = Res.y / Res.x;
	Vec2 c;

	glColor3f(Stog.Circ_color[0], Stog.Circ_color[1], Stog.Circ_color[2]);
	glBegin(GL_LINE_LOOP);
	

	for (int i = 0; i < 100; i++) {
		float t = 2.f * PI * i / 100;
		c.x = (circ * sin(t) * ResScale) + pos.x;
		c.y = circ * cos(t) + pos.y;
		
		glVertex2f(c.x, c.y);
		
	 }

	glEnd();
	return true;
}

uintptr_t DrawRadar(Vec2& pos) {

	Vec2 Res{ Res.x = GetSystemMetrics(SM_CXSCREEN), Res.y = GetSystemMetrics(SM_CYSCREEN) - 60.f };
	float ResScale = Res.y / Res.x;
	Vec2 c;
	if (!Stog.radarBackground) { glColor4f(Stog.radar_Circ_color[0], Stog.radar_Circ_color[1], Stog.radar_Circ_color[2], 0); }
	else {
		glColor4f(Stog.radar_Circ_color[0], Stog.radar_Circ_color[1], Stog.radar_Circ_color[2], Stog.radarAlpha / 10);
	}

	glBegin(GL_POLYGON);

	for (int i = 0; i < 100; i++) {
		float t = 2.f * PI * i / 100;
		c.x = (Stog.radarCirc * sin(t) * ResScale) + pos.x;
		c.y = Stog.radarCirc * cos(t) + pos.y;
		glVertex2f(c.x, c.y);
	}
	glEnd();	
	return true;
}

void DrawRadarBorder(Vec2& pos) {
	Vec2 Res{ Res.x = GetSystemMetrics(SM_CXSCREEN), Res.y = GetSystemMetrics(SM_CYSCREEN) - 60.f };
	float ResScale = Res.y / Res.x;
	Vec2 c;
		glBegin(GL_LINE_LOOP);
		glColor4f(Stog.Border_color[0], Stog.Border_color[1], Stog.Border_color[2], Stog.radarBorderAlpha / 10);
		for (int i = 0; i < 100; i++) {
			float t = 2.f * PI * i / 100;
			c.x = (Stog.radarCirc * sin(t) * ResScale) + pos.x;
			c.y = Stog.radarCirc * cos(t) + pos.y;
			glVertex2f(c.x, c.y);	
		}
		glEnd();
}

void DrawLocalPlayerDot(Vec2& pos) {
	glColor3f(Stog.player_dot_color[0], Stog.player_dot_color[1], Stog.player_dot_color[2]); // Red color for enemy dot
	glPointSize(Stog.PlayerSize); // Size of the dot
	glBegin(GL_POINTS);
	glVertex2f(pos.x, pos.y);
	glEnd();
}

void DrawNPCDot(const Vec2& position) {
	glColor3f(Stog.NPC_dot_color[0], Stog.NPC_dot_color[1], Stog.NPC_dot_color[2]); // Red color for enemy dot
	glPointSize(Stog.NPCSize); // Size of the dot
	glBegin(GL_POINTS);
	glVertex2f(position.x, position.y);
	glEnd();
}

void DrawRadarLine(const Vec2& position) {
	glLineWidth(1.f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(Stog.radarPos.x, Stog.radarPos.y);
	glVertex2f(position.x, position.y);
	glEnd();
}

uintptr_t DrawBox(int& health, Vec2& Pos, Vec2& HPos) {
	float height = ((Pos.y - HPos.y) * -1.f);
	float width = (height / 6.f);
	height += width / 1.1;
	float WTSHealth = (health / 100.f) * height;



	glColor3f(Stog.Box_color[0], Stog.Box_color[1], Stog.Box_color[2]); // default color - white
	glLineWidth(1.f); // default width

	glBegin(GL_LINE_STRIP); //btm left
	glVertex2f(Pos.x - width, Pos.y+width);
	glVertex2f(Pos.x - width, Pos.y);
	glVertex2f(Pos.x - (width / 2.f), Pos.y);
	glEnd();

	glBegin(GL_LINE_STRIP); //btm right
	glVertex2f(Pos.x + width, Pos.y + width);
	glVertex2f(Pos.x + width, Pos.y);
	glVertex2f(Pos.x + (width / 2.f), Pos.y);
	glEnd();

	glBegin(GL_LINE_STRIP); //top left
	glVertex2f(Pos.x - (width / 2.f), Pos.y + height);
	glVertex2f(Pos.x - width, Pos.y + height);
	glVertex2f(Pos.x - width, Pos.y + (height-width));
	glEnd();

	glBegin(GL_LINE_STRIP); //top right
	
	glVertex2f(Pos.x + (width / 2.f), Pos.y + height);
	glVertex2f(Pos.x + width, Pos.y + height);
	glVertex2f(Pos.x + width, Pos.y + (height - width));
	glEnd();
	

	glLineWidth(2.8f);
	glBegin(GL_LINE_STRIP); //Health Base
	glColor3f(1.f, 0.f, 0.f);
	glVertex2f(Pos.x - (width + 0.004), Pos.y);
	glVertex2f(Pos.x - (width + 0.004), Pos.y + height);
	glEnd();

	
	glBegin(GL_LINE_STRIP); //btm left
	glColor3f(0.f, 1.f, 0.f);
	glVertex2f(Pos.x - (width + 0.004), Pos.y);
	glVertex2f(Pos.x - (width + 0.004), Pos.y + WTSHealth);
	glEnd();
	return true;

}

uintptr_t DrawBones(Vec2& BPos1, Vec2& BPos2) {
	
	glColor3f(Stog.Bone_color[0], Stog.Bone_color[1], Stog.Bone_color[2]); // default color - white
	glLineWidth(1.f); // default width
	glBegin(GL_LINES); //btm left
	glVertex2f(BPos1.x, BPos1.y);
	glVertex2f(BPos2.x, BPos2.y);
	glEnd();
	
	return true;
}

Vec3 BoneMatrix(float* Matrix) {
	Vec3 Bone;
	Bone.x = Matrix[11];
	Bone.y = Matrix[3];
	Bone.z = Matrix[7];

	return Bone;
}


uintptr_t DrawBoneEsp(mem& m, uintptr_t& boneMatrix, float BoneNum1, float BoneNum2) {
	
	Vec2 BPos1;
	Vec2 BPos2;

	Matrix VM = m.readmem<Matrix>(addr.VM);

	const size_t pad = 0x30;

	uintptr_t BonePos1Addr = m.findMA(boneMatrix, BoneNum1 * pad);
	uintptr_t BonePos2Addr = m.findMA(boneMatrix, BoneNum2 * pad);
	Matrix BonePos1 = m.readmem<Matrix>(BonePos1Addr);
	Matrix BonePos2 = m.readmem<Matrix>(BonePos2Addr);

	Vec3 Bone1 = BoneMatrix(BonePos1.BM);
	Vec3 Bone2 = BoneMatrix(BonePos2.BM);
	
	if (WTS(Bone1, BPos1, VM.VM) && WTS(Bone2, BPos2, VM.VM)) {
		DrawBones(BPos1, BPos2);	
	}
	


	return true;
}


uintptr_t RadtoDeg(Rotate& r) {
	r.yaw *= (180 / PI);
	r.pitch *= (180 / PI);
	
	return true;
}

float SetAim(mem& m, Rotate r, float smoothVal, float aimFOV, uintptr_t& pitchAddr, uintptr_t& yawAddr) {

	Rotate smooth;
	smooth.pitch = m.readmem<float>(addr.pitchAddr); // current angles
	smooth.yaw = m.readmem<float>(addr.yawAddr);
	float pitch = (r.pitch - smooth.pitch); // difference btween current aingles and desired
	float yaw = (r.yaw - smooth.yaw);
	
	smooth.yaw += yaw / smoothVal;
	smooth.pitch += pitch / smoothVal;
	if (yaw < aimFOV && yaw > -aimFOV && pitch < aimFOV && pitch > -aimFOV) {
		m.writemem<float>(addr.pitchAddr, smooth.pitch);
		m.writemem<float>(addr.yawAddr, smooth.yaw);
	}

	return true;
}

uintptr_t Aim(mem& m, Vec3 Head, Vec3 Origin, float& c, Rotate& r, uintptr_t& pitchAddr, uintptr_t& yawAddr) {

	r.pitch = m.readmem<float>(addr.pitchAddr);
	r.yaw = m.readmem<float>(addr.yawAddr);
	
	Vec3 Diff = { Head.x - Origin.x, Head.y - Origin.y, Head.z - Origin.z}; // sets player cords to 0,0,0
	c = sqrt(Diff.x * Diff.x + Diff.y * Diff.y + Diff.z * Diff.z);
	
	r.yaw = atan2(Diff.y, Diff.x);
	r.pitch = -asin(Diff.z/c);

	return (RadtoDeg(r)), c;
	
	
}

