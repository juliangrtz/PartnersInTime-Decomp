#include <nitro/os_arena.h>

extern int data_02062f58;
extern int data_02062f5c;
extern u32 func_02039b34(void);
extern void OS_SetProtectionRegion1(u32 value);
extern void OS_SetProtectionRegion2(u32 value);

void OS_InitArena(void)
{
    if (data_02062f58) return;
    data_02062f58 = 1;
    OS_SetArenaHi(OS_ARENA_MAIN, OS_GetInitArenaHi(OS_ARENA_MAIN));
    OS_SetArenaLo(OS_ARENA_MAIN, OS_GetInitArenaLo(OS_ARENA_MAIN));
    OS_SetArenaLo(OS_ARENA_MAIN_EX, 0);
    OS_SetArenaHi(OS_ARENA_MAIN_EX, 0);
    OS_SetArenaHi(OS_ARENA_ITCM, OS_GetInitArenaHi(OS_ARENA_ITCM));
    OS_SetArenaLo(OS_ARENA_ITCM, OS_GetInitArenaLo(OS_ARENA_ITCM));
    OS_SetArenaHi(OS_ARENA_DTCM, OS_GetInitArenaHi(OS_ARENA_DTCM));
    OS_SetArenaLo(OS_ARENA_DTCM, OS_GetInitArenaLo(OS_ARENA_DTCM));
    OS_SetArenaHi(OS_ARENA_SHARED, OS_GetInitArenaHi(OS_ARENA_SHARED));
    OS_SetArenaLo(OS_ARENA_SHARED, OS_GetInitArenaLo(OS_ARENA_SHARED));
    OS_SetArenaHi(OS_ARENA_WRAM_MAIN, OS_GetInitArenaHi(OS_ARENA_WRAM_MAIN));
    OS_SetArenaLo(OS_ARENA_WRAM_MAIN, OS_GetInitArenaLo(OS_ARENA_WRAM_MAIN));
}

void OS_InitArenaEx(void)
{
    OS_SetArenaHi(OS_ARENA_MAIN_EX, OS_GetInitArenaHi(OS_ARENA_MAIN_EX));
    OS_SetArenaLo(OS_ARENA_MAIN_EX, OS_GetInitArenaLo(OS_ARENA_MAIN_EX));
    if (!data_02062f5c || *(vu16 *)0x027FFC40 == 2 || (func_02039b34() & 3) == 1) {
        OS_SetProtectionRegion1(0x0200002B);
        OS_SetProtectionRegion2(0x023E0021);
    }
}
