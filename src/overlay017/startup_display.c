#include <game/battle_ai.h>
#include <game/overlay017_attack.h>

#include "attack_state.h"

enum Overlay17StartupDisplayConstant {
    OVERLAY17_DISPLAY_BUFFER_POINTER_OFFSET = 0x68,
    OVERLAY17_DISPLAY_PATTERN_OFFSET = 0x6200,
    OVERLAY17_DISPLAY_PATTERN_WORD_COUNT = 8,
    OVERLAY17_DISPLAY_COPY_SIZE = 0x20,
};

extern void *func_ov002_020725a4(
    void (*callback)(), const void *argument_1,
    void *argument_2, u32 argument_3);
extern int func_0202cc58(const void *source, void *destination, u32 size);
extern int func_0202cd2c(const void *source, void *destination, u32 size);
extern void func_0200e678(void);
extern int func_0200e6f8(int argument_0, int argument_1,
                         int argument_2, int argument_3,
                         int argument_4, int argument_5,
                         int argument_6, const void *argument_7,
                         int argument_8, int argument_9);
extern u8 data_ov017_020c6c98[];

/* Metrowerks emits C functions in reverse source order. */
int Overlay17Attack_ConfigureDisplay(int argument) {
    return func_0200e6f8(argument, 0, 6, 8, 0, 1, 0,
                         data_ov017_020c6c98, -15, 0);
}

void Overlay17Attack_FinalizeDisplay(void) {
    func_0200e678();
}

void Overlay17Attack_InitializeDisplayPattern(void) {
    int copy_index;
    int word_offset;
    u32 copy_size;
    u32 value;
    int word_index;
    u32 pattern;
    u32 *cursor;

    value = 0x19110901;
    word_index = 0;
    pattern = *(u32 *)(gBattleContext +
                       OVERLAY17_DISPLAY_BUFFER_POINTER_OFFSET) +
              OVERLAY17_DISPLAY_PATTERN_OFFSET;
    cursor = (u32 *)pattern;

    do {
        ++word_index;
        *cursor = value;
        value += 0x20202020;
        ++cursor;
    } while (word_index < OVERLAY17_DISPLAY_PATTERN_WORD_COUNT);

    copy_index = 1;
    word_offset = OVERLAY17_DISPLAY_PATTERN_WORD_COUNT;
    copy_size = OVERLAY17_DISPLAY_COPY_SIZE;
    do {
        void *destination;

        /* Preserve the original indexed address calculation. */
        asm {
            add destination, cursor, word_offset, lsl #2
        }

        if (pattern < (u32)destination) {
            func_0202cd2c((const void *)pattern, destination, copy_size);
        } else {
            func_0202cc58((const void *)pattern, destination, copy_size);
        }
        ++copy_index;
        word_offset += OVERLAY17_DISPLAY_PATTERN_WORD_COUNT;
    } while (copy_index < OVERLAY17_DISPLAY_PATTERN_WORD_COUNT);

    data_ov002_020c0710->setup_flags |= 2;
}

void *Overlay17Attack_QueueDisplayInitialization(void) {
    return func_ov002_020725a4(
        Overlay17Attack_InitializeDisplayPattern, 0, 0, 0);
}
