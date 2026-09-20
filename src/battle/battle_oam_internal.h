#ifndef PIT_BATTLE_OAM_INTERNAL_H
#define PIT_BATTLE_OAM_INTERNAL_H

#include <game/battle_callback_model.h>
#include <game/sprite_output.h>
typedef struct BattleOamTransform {
    s16 scale_x, scale_y;
    u16 rotation;
    s8 offset_x, offset_y;
    u32 flags;
} BattleOamTransform;
typedef struct BattleNumberDisplay {
    u32 flags;
    BattleOamTransform transform;
    u32 value;
    const void *source;
    u8 source_width;
    s8 leading;
    u8 padding[2];
} BattleNumberDisplay;
typedef struct BattleOamDisplay {
    u32 flags;
    BattleOamTransform transform;
    const GameOamEntry *objects;
    u16 count;
    u8 padding[2];
} BattleOamDisplay;
typedef char OamTransformSize[sizeof(BattleOamTransform) == 12 ? 1 : -1];
typedef char NumberDisplaySize[sizeof(BattleNumberDisplay) == 28 ? 1 : -1];
typedef char OamDisplaySize[sizeof(BattleOamDisplay) == 24 ? 1 : -1];
/* OAM composition takes the compact 12-byte transform, not the 64-byte
   matrix/position BattleSpriteTransform used by model effects. */
void func_ov002_0206acbc(GameOamEntry *, u8 *, u8 *, const GameOamEntry *, u8, s16, s16,
                         BattleOamTransform *, u8);
void BattleOam_DrawNumber(GameOamEntry *, u8 *, u8 *, int, int, int, int, BattleOamTransform *);
void BattleNumberModel_Draw(BattleModel *, void *, u8 *, u8 *);
void BattleCachedOamModel_Draw(BattleModel *, void *, u8 *, u8 *);

#endif
