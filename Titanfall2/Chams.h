#pragma once
#include <Windows.h>
#include "T2.h"
uintptr_t ChamsON(mem& m, uintptr_t& localPlayer) {
    
    for (uintptr_t i = 0; i < 100; i++) {
        uintptr_t entity = m.readmem<uintptr_t>(addr.entList + i * 0x20);
        uintptr_t player = m.readmem<uintptr_t>(entity + off.playerName);

        if (entity == NULL) { break; }
        if (player != 125780153691248) { break; }
        if (entity == localPlayer) { continue; }

        uintptr_t chams = m.writemem<int>(entity + off.glow, 7);
      }
    return true;
}
uintptr_t ChamsOFF(mem& m, uintptr_t& localPlayer) {
    for (uintptr_t i = 0; i < 100; i++) {
        uintptr_t entity = m.readmem<uintptr_t>(addr.entList + i * 0x20);
        uintptr_t player = m.readmem<uintptr_t>(entity + off.playerName);

        if (entity == NULL) { break; }
        if (player != 125780153691248) { break; }
        if (entity == localPlayer) { continue; }

        uintptr_t chams = m.writemem<int>(entity + off.glow, 2);
    }

    return true;
}