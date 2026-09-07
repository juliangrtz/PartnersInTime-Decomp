#ifndef NITRO_OS_ARENA_H
#define NITRO_OS_ARENA_H

#include <nitro.h>

typedef enum OsArenaId {
    OS_ARENA_MAIN = 0,
    OS_ARENA_MAIN_EX = 2,
    OS_ARENA_ITCM = 3,
    OS_ARENA_DTCM = 4,
    OS_ARENA_SHARED = 5,
    OS_ARENA_WRAM_MAIN = 6
} OsArenaId;

void OS_InitArena(void);
void OS_InitArenaEx(void);
void *OS_GetInitArenaHi(OsArenaId arena);
void *OS_GetInitArenaLo(OsArenaId arena);
void OS_SetArenaHi(OsArenaId arena, void *high);
void OS_SetArenaLo(OsArenaId arena, void *low);

#endif
