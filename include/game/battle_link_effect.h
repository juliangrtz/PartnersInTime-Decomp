#ifndef PIT_GAME_BATTLE_LINK_EFFECT_H
#define PIT_GAME_BATTLE_LINK_EFFECT_H
#include <nitro.h>
/* The chain uses 16-byte particles with Q8 screen coordinates and velocity. */
typedef struct BattleLinkPoint {
    int x, y, vx, vy;
} BattleLinkPoint;
typedef struct BattleLinkEffectState {
    u16 end_mass, color;
    u8 spacing, count;
    s8 screen_y, source_x, source_y, target_x, target_y, depth;
    u8 source, target, width, padding;
} BattleLinkEffectState;
typedef struct BattleLinkEffectTask {
    struct BattleLinkEffectTask *next;
    void (*callback)(struct BattleLinkEffectTask *);
    struct BattleLinkEffectTask **owner;
    BattleLinkEffectState state;
} BattleLinkEffectTask;
typedef char BattleLinkPointSize[sizeof(BattleLinkPoint) == 16 ? 1 : -1];
typedef char BattleLinkEffectStateSize[sizeof(BattleLinkEffectState) == 16 ? 1 : -1];
typedef char BattleLinkEffectTaskSize[sizeof(BattleLinkEffectTask) == 28 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void BattleLinkEffect_Update(BattleLinkEffectTask *task);
void BattleLinkEffect_SimulateAndDraw(BattleLinkPoint *points, unsigned count, int spacing,
                                      int end_mass, int depth, int screen_y, u16 color, u8 width);
#ifdef __cplusplus
}
#endif
#endif
