#include <game/battle_link_effect.h>
#include <game/battle_context.h>
typedef struct LinkOwnerView {
    u8 prefix[54152];
    BattleLinkEffectTask *task;
} LinkOwnerView;
#define OWNER ((LinkOwnerView *)gBattleContext)
void func_02038448(const void *, u32, u32);
void BattleLinkEffect_UploadTileMap(void) { func_02038448((const void *)0x0689e000, 0, 1536); }
void BattleLinkEffect_DetachTarget(void) {
    if (OWNER->task)
        OWNER->task->state.target = 0;
}
void BattleLinkEffect_Stop(void) {
    if (OWNER->task)
        OWNER->task->callback = 0;
}
