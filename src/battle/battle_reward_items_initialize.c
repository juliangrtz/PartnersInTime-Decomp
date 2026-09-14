#include "battle_reward_items_internal.h"
#include <game/heap.h>
extern int data_ov002_020c071c;
void func_0202cbd4(void *, int, u32);
void func_ov002_02076ac8(void);
void func_ov002_0206dfd0(void);

void BattleRewardItems_Initialize(void) {
    data_ov002_020c06a4 = GameHeap_Allocate(data_ov002_020c071c, 1344, 0, 1);
    func_0202cbd4(data_ov002_020c06a4, 0, 1344);
    func_ov002_02076ac8();
    data_ov002_020c06a4->flags.bits.done = 0;
    data_ov002_020c06a4->flags.bits.load_phase = 0;
    data_ov002_020c06a4->update = func_ov002_0206dfd0;
}
