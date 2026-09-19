/*
 * OS arena bounds (ARM9 resident, 0x0203A0B4-0x0203A1C4).
 *
 * Reading and moving an arena's low and high water marks, which is how the
 * allocators above it carve memory out.
 */

#include <nitro/os_arena.h>
extern int data_02062f5c;
extern u32 func_02039b34(void);
void *OS_GetInitArenaLo(OsArenaId arena) {
    switch (arena) {
    case OS_ARENA_MAIN:
        return (void *)0x020cbfe0;
    case OS_ARENA_MAIN_EX:
        if (!data_02062f5c || (func_02039b34() & 3) == 1)
            return 0;
        return (void *)0x02400000;
    case OS_ARENA_ITCM:
        return (void *)0x01ff87a0;
    case OS_ARENA_DTCM:
        return (void *)0x027e0060;
    case OS_ARENA_SHARED:
        return (void *)0x027ff000;
    case OS_ARENA_WRAM_MAIN:
        return (void *)0x037f8000;
    default:
        return 0;
    }
}
/* Keep the indexed shared-RAM base separate from the arena-table offset. */
typedef struct ArenaSharedView {
    u8 prefix[0xffda0];
    void *low[9];
    void *high[9];
} ArenaSharedView;
typedef char ArenaSharedViewSizeCheck[(sizeof(ArenaSharedView) == 0xffde8) ? 1 : -1];
void OS_SetArenaHi(OsArenaId arena, void *high) {
    ((ArenaSharedView *)((u8 *)0x02700000 + arena * 4))->high[0] = high;
}
void OS_SetArenaLo(OsArenaId arena, void *low) {
    ((ArenaSharedView *)((u8 *)0x02700000 + arena * 4))->low[0] = low;
}
