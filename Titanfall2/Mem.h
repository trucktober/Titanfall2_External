#pragma once
#include <Windows.h>
#include <vector>

class mem {
public:
	mem();
	~mem();

	template <class v>
	v writemem(uintptr_t addr, v val) {
		WriteProcessMemory(h, (BYTE*)addr, &val, sizeof(val), nullptr);
		return val;
	}

	template <class v>
	v readmem(uintptr_t addr) {
		v val;
		ReadProcessMemory(h, (BYTE*)addr, &val, sizeof(val), NULL);
		return val;
	}

	uintptr_t module(uintptr_t procID, const wchar_t* modName);
	uintptr_t process(const wchar_t* proc);

	uintptr_t findDMA(uintptr_t addr, std::vector<unsigned int> offsets) {

		for (unsigned int i = 0; i < offsets.size(); ++i) {
			ReadProcessMemory(h, (BYTE*)addr, &addr, sizeof(addr), 0);
			addr += offsets[i];
		}
		return addr;

	}
	template <class v>
	uintptr_t VirtProtc(uintptr_t addr, v val) {
		DWORD protc;
		VirtualProtectEx(h, reinterpret_cast<LPVOID>(addr), val, PAGE_EXECUTE_READWRITE, &protc);
		WriteProcessMemory(h, (BYTE*)addr, &val, sizeof(val), nullptr);
		VirtualProtectEx(h, reinterpret_cast<LPVOID>(addr), val, protc, &protc);
		return true;
	}

	void nopEx(BYTE* src, unsigned int len) {
		BYTE* Array = new BYTE[len];
		memset(Array, 0x90, len);
		patchEx(src, Array, len);
		delete[] Array;
	}

	void patchEx(BYTE* src, BYTE* dst, unsigned int len) { // WORK HERE
		DWORD protect;
		VirtualProtectEx(h, src, len, PAGE_EXECUTE_READWRITE, &protect);
		WriteProcessMemory(h,src, dst, len, nullptr);
		VirtualProtectEx(h, src, len, protect, &protect);
	}

	uintptr_t findMA(uintptr_t addr, uintptr_t offsets);
	uintptr_t getProcID() const { return procID; };
private:
	HANDLE h;
	uintptr_t procID;
};

