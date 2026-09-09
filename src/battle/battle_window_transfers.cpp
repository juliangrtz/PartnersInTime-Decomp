extern "C" {
#include <game/battle_window.h>
extern void (*data_ov002_020be8b0[])(GameSpriteAnimator *, GameSpriteAnimation *);
/* The ITCM routine builds store-multiple instructions from two adjacent templates. */
void GameImage_FillWordRectITCM(void *, u32, u32, u32);
void func_0202cbd4(void *, int, u32);
void func_020106a4(const void *, u32, void *, u32);
void func_0202cf80(const void *, void *, int, int);
void func_02021654(GameSpriteAnimator *, GameSpriteAnimation *);
void func_ov002_0206faec(BattleWindowManager *, const GameWindow *);
void func_ov002_0206fb38(BattleWindowManager *);
void func_ov002_02070cdc(BattleWindowAnimator *, GameSpriteAnimation *);
}

void BattleWindow_UploadTextTilesTask(BattleWindowTransferTask *task)
{
    const BattleWindowState *state = (const BattleWindowState *)task->source;
    func_020106a4(state->upload_source, (state->upload_rows << 16) | state->upload_columns,
                  state->upload_destination, state->upload_pitch);
}

void BattleWindow_UploadSubTilemapTask(BattleWindowTransferTask *task)
{
    func_ov002_0206fb38((BattleWindowManager *)task->destination);
}

void BattleWindow_UploadSubWindowTask(BattleWindowTransferTask *task)
{
    func_ov002_0206faec((BattleWindowManager *)task->destination, (const GameWindow *)task->source);
}

void BattleWindow_ResetSubScrollTask(BattleWindowTransferTask *task)
{
    *(vu32 *)0x04001010 = 0;
    BattleWindow_UploadSubTilemapTask(task);
}

void BattleWindow_CopyTextureTask(BattleWindowTransferTask *task)
{
    func_0202cf80(task->source, task->destination, (u8)task->size, (int)task->size >> 8);
}

void BattleWindow_FillTextureTask(BattleWindowTransferTask *task)
{
    GameImage_FillWordRectITCM(task->destination, (task->size & 0xffff0000) | 32, task->size & 0xffff,
                               task->fill_value);
}

BattleWindowAnimator *BattleWindowAnimator_Init(BattleWindowAnimator *animator)
{
    GameSpriteAnimator_Init(&animator->base);
    animator->base.vtable = data_ov002_020be8b0;
    func_0202cbd4(&animator->manager, 0, (u8 *)(animator + 1) - (u8 *)&animator->manager);
    func_0202cbd4(&animator->base.allocation[0], 0, sizeof(GameSpriteAllocation));
    animator->base.state.bits.main_allocated = 1;
    animator->base.allocation[0].offset = 32;
    GameSpriteAnimator_RemapTiles(&animator->base, 0);
    animator->base.state.bits.main_loaded = 1;
    return animator;
}

void BattleWindowAnimator_CopySubOam(BattleWindowAnimator *animator, GameOamEntry *oam)
{
    const u32 *source = (const u32 *)animator->base.oam[1];
    u32 *destination = (u32 *)&oam[127];
    while (source[1]) {
        destination[0] = source[0];
        destination[1] = source[1];
        source += 2;
        destination -= 2;
    }
}

void BattleWindowAnimator_Draw(BattleWindowAnimator *animator, GameSpriteAnimation *animation)
{
    if (!animation->flags.bits.screen)
        func_ov002_02070cdc(animator, animation);
    else
        func_02021654(&animator->base, animation);
}
