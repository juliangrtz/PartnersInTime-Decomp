extern "C" {
#include <game/battle_window_upload.h>
#include <nitro.h>
extern u8 *gBattleContext;
void DC_FlushRange(const void *, u32);
void func_02038448(const void *, u32, u32);
void *func_ov002_02072508(void (*)(BattleWindowTransferTask *), const void *, void *, u32);
extern GameOamEntry data_02060740[];
}
enum { BATTLE_SUB_BLEND_SOURCE_OFFSET = 54 };
#define UPLOAD_FLAGS ((BattleWindowUploadFlags *)&manager->flags)
extern "C" void BattleWindow_ScheduleSubUploads(BattleWindowManager *manager)
{
    /* Preserve the native packed count test, including the high main-count bit. */
    u16 counts = (manager->base.state.bits.sub_count << 8) | manager->base.state.bits.main_count;
    int visible = counts >> 4;
    if (!visible && UPLOAD_FLAGS->bits.pending) {
        func_ov002_02072508(BattleWindow_ResetSubScrollTask, 0, manager, 0);
        manager->flags &= ~1;
        return;
    }
    if (visible) {
        manager->flags &= ~1;
        BattleWindowAnimator_CopySubOam((BattleWindowAnimator *)manager->base.animator, data_02060740);
        *(u16 *)(gBattleContext + BATTLE_SUB_BLEND_SOURCE_OFFSET) = 0;
        func_ov002_02072508(BattleWindow_UploadSubTilemapTask, 0, manager, 0);
        for (GameWindow *window = manager->base.sub_windows; window->next != (GameWindow *)-1;
             window = window->next)
            func_ov002_02072508(BattleWindow_UploadSubWindowTask, window, manager, 0);
    }
}
extern "C" void BattleWindow_UploadSubTilemap(BattleWindowManager *manager)
{
    DC_FlushRange(manager->base.sub_tilemap, manager->base.sub_map_size);
    func_02038448(manager->base.sub_tilemap, 0, manager->base.sub_map_size);
}
extern "C" void BattleWindow_UploadSubWindow(BattleWindowManager *manager, const GameWindow *window)
{
    *(vu32 *)0x04001010 =
        (manager->base.sub_origin_x & 0x1ff) | (((u32)manager->base.sub_origin_y << 16) & 0x1ff0000);
    GameWindow_Upload(&manager->base, (GameWindow *)window);
}
extern "C" void BattleWindow_SetLinkFlag30(GameWindow *window, u8 enabled)
{
    ((BattleWindowLinkFlags *)&window->links[0].state)->bits.flag_30 = enabled;
    ((BattleWindowLinkFlags *)&window->links[1].state)->bits.flag_30 = enabled;
}
extern "C" int BattleWindow_IsUploadReady(BattleWindowManager *manager, s16 index)
{
    /* The incremental texture upload reaches its ready phase at state two. */
    return manager->windows[index].unknown_d0[0] >= 2;
}
