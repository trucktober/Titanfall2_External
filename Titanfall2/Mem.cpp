#pragma once
#include "Mem.h"
#include <Windows.h>
#include <tlhelp32.h>

mem::mem() {
	h = NULL;
}
mem::~mem() {
	CloseHandle(h);
}

uintptr_t mem::process(const wchar_t* proc) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, proc)) {
					procID = procEntry.th32ProcessID;
					CloseHandle(hSnap);
					h = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);
					return procID;
				}
			} while (Process32Next(hSnap, &procEntry));
		}	
	}	
}

uintptr_t mem::module(uintptr_t procID, const wchar_t* modName) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					return (uintptr_t)modEntry.hModule;
					CloseHandle(hSnap);
					
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
}

uintptr_t mem::findMA(uintptr_t addr, uintptr_t offsets) {
		addr += offsets;
	return addr;
}
