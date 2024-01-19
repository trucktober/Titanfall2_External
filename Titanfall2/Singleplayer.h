#pragma once
#include <Windows.h>
#include "T2.h"
#include "S_T2.h"
#include "Vector Structor.h"
#include "Mem.h"
#include <math.h>
#define PI 3.14159265359
uintptr_t Fly(mem& m) {
	uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
	uintptr_t ZoriginAddr = player + Soff.Zoff;
	float Zorigin = m.readmem<float>(player + Soff.Zoff);
	/*std::cout << std::dec << Zorigin << std::endl;*/
	Vec3 origin = m.readmem<Vec3>(Saddr.origin);

	m.VirtProtc(ZoriginAddr, Zorigin += Stog.jumpMul);

	return true;
}

void resetVelZ(mem& m) {
	uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
	float vel = m.writemem(player + Soff.ZVelOff, 0);
}

void resetVelAll(mem& m) {
	uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
	float velX = m.writemem(player + Soff.XVelOff, 0);
	float velY = m.writemem(player + Soff.YVelOff, 0);
	float velZ = m.writemem(player + Soff.ZVelOff, 0);
}

void PointToFly(Rotate& r, mem& m) {
	uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
	uintptr_t player2 = m.readmem<uintptr_t>(addr.localplayer);
	uintptr_t pitchAddr = m.findMA(player2, off.pitch);
	uintptr_t yawAddr = m.findMA(player2, off.yaw);
	Vec3 origin = m.readmem<Vec3>(player + Soff.originOff);
	r.pitch = (m.readmem<float>(pitchAddr)) * -1;
	r.yaw = m.readmem<float>(yawAddr);

	r.yaw *= (PI / 180);
	r.pitch *= (PI / 180);

	Vec3 distance;
	distance.x = cosf(r.yaw) * Stog.ptfSpeed;
	distance.y = sinf(r.yaw) * Stog.ptfSpeed;
	distance.z = sin(r.pitch) * Stog.ptfSpeed;
	m.VirtProtc(player + Soff.Yoff, origin.y + distance.y);
	m.VirtProtc(player + Soff.Xoff, origin.x + distance.x);
	m.VirtProtc(player + Soff.Zoff, origin.z + distance.z);

}

void Fly(mem& m, Rotate& r) {
	uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
	uintptr_t player2 = m.readmem<uintptr_t>(addr.localplayer);
	uintptr_t yawAddr = m.findMA(player2, off.yaw);
	Vec3 origin = m.readmem<Vec3>(player + Soff.originOff);
	if (Stog.w || Stog.a || Stog.s || Stog.d) { Stog.isMoving = true; }
	else { Stog.isMoving = false; }

	if (Stog.activatePTF) {
		m.VirtProtc<float>(player + Soff.Zoff, origin.z + -1.f * Stog.flySpeed);
	}
	if (Stog.addJump) {
		m.VirtProtc<float>(player + Soff.Zoff, origin.z + 1.f * Stog.flySpeed);
	}



	if (Stog.isMoving) {
		r.yaw = m.readmem<float>(yawAddr);
		if (Stog.w) {
			r.yaw += (Stog.a) ? 45 : (Stog.d) ? -45 : 0;
		}
		if (Stog.s) {
			r.yaw += ((Stog.a) ? -45 : (Stog.d) ? 45 : 0) - 180;
		}
		if (Stog.d && !Stog.s && !Stog.w) {
			r.yaw -= 90;
		}
		if (Stog.a && !Stog.s && !Stog.w) {
			r.yaw += 90;
		}

		r.yaw *= (PI / 180);

		Vec2 distance;
		distance.x = cosf(r.yaw) * Stog.flySpeed;
		distance.y = sinf(r.yaw) * Stog.flySpeed;
		m.VirtProtc(player + Soff.Yoff, origin.y + distance.y);
		m.VirtProtc(player + Soff.Xoff, origin.x + distance.x);
	}

}

void noClip(mem& m, Rotate& r) {
	uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
	uintptr_t player2 = m.readmem<uintptr_t>(addr.localplayer);
	Vec3 currentVel = m.readmem<Vec3>(player + Soff.XVelOff);
	uintptr_t yawAddr = m.findMA(player2, off.yaw);
	Vec3 origin = m.readmem<Vec3>(player + Soff.originOff);
	if (Stog.w || Stog.a || Stog.s || Stog.d) { Stog.isMoving = true; }
	else { Stog.isMoving = false; }
	if (currentVel.x != 0 && currentVel.y != 0){
		if (Stog.isMoving) {
			r.yaw = m.readmem<float>(yawAddr);
			if (Stog.w) {
				r.yaw += (Stog.a) ? 45 : (Stog.d) ? -45 : 0;
			}
			if (Stog.s) {
				r.yaw += ((Stog.a) ? -45 : (Stog.d) ? 45 : 0) - 180;
			}
			if (Stog.d && !Stog.s && !Stog.w) {
				r.yaw -= 90;
			}
			if (Stog.a && !Stog.s && !Stog.w) {
				r.yaw += 90;
			}

			r.yaw *= (PI / 180);

			Vec2 distance;
			distance.x = cosf(r.yaw) * 1;
			distance.y = sinf(r.yaw) * 1;
			m.VirtProtc(player + Soff.Yoff, origin.y + distance.y);
			m.VirtProtc(player + Soff.Xoff, origin.x + distance.x);
		}
	}
}





void SpeedHack(mem& m, Rotate& r) { // BROKEN
    uintptr_t player = m.readmem<uintptr_t>(Saddr.localPlayer);
    Vec3 currentVel = m.readmem<Vec3>(player + Soff.XVelOff);
	Vec3 origin = m.readmem<Vec3>(player + Soff.originOff);

	currentVel = {currentVel.x * 5, currentVel.y * 5, currentVel.z * 5};
	if (currentVel.x < 500.f ) {
		currentVel.x = 500.f;
		m.VirtProtc(player + Soff.XVelOff, currentVel.x);
	}
	else if (currentVel.x > -500.f) {
		currentVel.x = -500.f;
		m.VirtProtc(player + Soff.XVelOff, currentVel.x);
	}
	if (currentVel.y < 500.f ) {
		currentVel.y = 500.f;
		m.VirtProtc(player + Soff.YVelOff, currentVel.y);
	}
	else if (currentVel.y > -500.f) {
		currentVel.y = -500.f;
		m.VirtProtc(player + Soff.YVelOff, currentVel.y);
	}
	if (currentVel.z < 500.f ) {
		currentVel.z = 500.f;
		m.VirtProtc(player + Soff.ZVelOff, currentVel.z);
	}
	else if (currentVel.z > -500.f) {
		currentVel.z = -500.f;
		m.VirtProtc(player + Soff.ZVelOff, currentVel.z);
	}
	// multiple velocity by a 5, break loop if above 500

}

void patchVel(mem& m) {
	if (!Stog.longJump)
	m.patchEx((BYTE*)(Saddr.velocity), (BYTE*)"\xF3\x0F\x11\x77\x08", 5);
}

void S_Aimbot(mem& m, Rotate& r, Vec3 eyePos, Vec3 Enemy_eyePos) {
	const float RadtoDeg = (180 / PI);
	uintptr_t lclPlayerAddr = m.readmem<uintptr_t>(addr.localplayer);
	Rotate smooth;
	
	r.yaw = m.readmem<float>(lclPlayerAddr + off.yaw);
	r.pitch = m.readmem<float>(lclPlayerAddr + off.pitch);
	smooth.yaw = m.readmem<float>(lclPlayerAddr + off.yaw);
	smooth.pitch = m.readmem<float>(lclPlayerAddr + off.pitch);
	
	Vec3 Diff = { Enemy_eyePos.x - eyePos.x, Enemy_eyePos.y - eyePos.y, Enemy_eyePos.z - eyePos.z };
	float c = sqrt(Diff.x * Diff.x + Diff.y * Diff.y + Diff.z * Diff.z);
	r.yaw = atan2(Diff.y, Diff.x) * RadtoDeg;
	r.pitch = -asin(Diff.z / c) * RadtoDeg;
	
	if (c < tog.distance) {
		m.VirtProtc(lclPlayerAddr + off.yaw, smooth.yaw += (r.yaw - smooth.yaw) / tog.smoothVal);
		m.VirtProtc(lclPlayerAddr + off.pitch, smooth.pitch += (r.pitch - smooth.pitch) / tog.smoothVal);
	}

}
