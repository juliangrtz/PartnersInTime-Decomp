#include "pocket_chomp_internal.h"

extern "C" {
int PocketChompAdult_SelectEffect(Overlay18AttackObjectController *controller, int primary, int secondary)
{
    int alternate = controller->flag_bits.mode == 2 || controller->flag_bits.mode == 3;
    return func_ov002_02072004(controller->object, 0, alternate ? secondary : primary, 0, 0);
}

u32 PocketChompAdult_BeginEntry(Overlay18AttackObjectController *controller)
{
    BattleSceneObject *object = controller->object;
    BattlePartyActor *actor = (BattlePartyActor *)controller->source;
    controller->flags &= ~0x800;
    func_ov018_020c2e50(controller, 516);
    BattleSceneObject_AdjustPosition(object, -48 - object->x, actor->actor.unk_01a - object->y, -object->z);
    int dx = actor->actor.unk_018 + 48;
    BattleSceneObject_MoveBy(object, 1, dx, 0, 0, (dx << 8) / 768);
    BattleSound_Play(61, 0, 0, 0);
    u32 result = (controller->flags & ~0x7c0) | (POCKET_CHOMP_SUPPORT_ENTRY << 6);
    controller->flags = result;
    return result;
}

int PocketChompAttack_ScaleTargetHeight(int height)
{
    BattleSceneObject *target = BattleSceneObject_GetById(data_ov002_020c0710->target_id);
    return Overlay10Motion_ScaleHeight(target, height, 64);
}
}
