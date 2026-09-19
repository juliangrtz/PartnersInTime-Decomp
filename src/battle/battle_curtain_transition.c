#include <game/battle_context.h>
#include <game/battle_curtain_transition.h>

enum BattleCurtainTransitionOffset {
    BATTLE_CURTAIN_RUNTIME_FLAGS_OFFSET = 0xD3A0
};
enum BattleCurtainTransitionFlag {
    BATTLE_CURTAIN_ACTIVE = 1 << 6
};
void BattleCurtainTransition_UpdateTask(BattleCurtainTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleCurtainTransitionTask *BattleCurtainTransition_InitializeTask(
    BattleCurtainTransitionTask *task) {
    task->progress = 0;
    task->callback = BattleCurtainTransition_UpdateTask;
    return task;
}

void BattleCurtainTransition_UpdateTask(BattleCurtainTransitionTask *task) {
    BattleCurtainTransition_Draw(task->progress * 2);
    ++task->progress;
    if (task->progress >= 32) {
        *(u32 *)(gBattleContext + BATTLE_CURTAIN_RUNTIME_FLAGS_OFFSET) &=
            ~BATTLE_CURTAIN_ACTIVE;
        task->callback = 0;
    }
}

#define REG32(address) (*(volatile u32 *)(address))
extern const s16 FX_SinCosTable_[];
extern const u8 data_ov002_020c03d4[];
extern const int data_ov002_020c052c[];
void func_02036cc0(const void *matrix);
static inline void Vertex(s16 x, s16 y, s16 z) {
    REG32(0x0400048c) = (u16)x | ((u16)y << 16);
    REG32(0x0400048c) = (u16)z;
}
void BattleCurtainTransition_Draw(int progress) {
    int ring, segment;
    REG32(0x040004a8) = 0;
    REG32(0x04000440) = 2;
    func_02036cc0(data_ov002_020c052c);
    REG32(0x04000480) = 0;
    for (ring = 0; ring < 50; ++ring) {
        int alpha = ring + 22 - progress;
        if (alpha >= 1) {
            if ((u32)alpha >= 22)
                alpha = 31;
            else
                alpha = data_ov002_020c03d4[alpha];
            REG32(0x040004a4) = 0x020000c0 | (alpha << 16);
            REG32(0x04000500) = 3;
            for (segment = 0; segment <= 32; ++segment) {
                u16 angle = (segment << 16) / 32;
                int sine = FX_SinCosTable_[(angle >> 4) * 2];
                int cosine = FX_SinCosTable_[(angle >> 4) * 2 + 1];
                int x = ring * (sine << 3), next_x = x + (sine << 3);
                int y = ring * (cosine << 2), next_y = y + (cosine << 2);
                Vertex(((x << 8) / 4096) / 4, ((y << 8) / 4096) / 4, 80);
                Vertex(((next_x << 8) / 4096) / 4, ((next_y << 8) / 4096) / 4, 80);
            }
            REG32(0x04000504) = 0;
        }
    }
}
