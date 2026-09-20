/*
 * Pause status numbers (overlay 7, 0x02078AC0-0x02078BEC).
 *
 * Refreshes one saved stat and submits its configured decimal digit sprites.
 * Level and experience share a word; the next-level word also holds clothing.
 */
#include <game/save_data.h>
#include <game/overlay005_resource.h>

struct PauseStatusNumberTask {
    u8 unknown_00[40];
    u8 sprite_index, member, stat, unknown_2b;
    int digits, value, divisor, x, y;
    u8 unknown_40[8];
};
typedef char TaskSize[sizeof(PauseStatusNumberTask) == 72 ? 1 : -1];
extern "C" {
extern Overlay5ObjectSprite *data_ov007_02090900[];
void func_ov007_0207a680(Overlay5ObjectSprite *, int, int, int, int, int, int, int, int, int);
}
extern "C" void PauseStatus_UpdateNumber(PauseStatusNumberTask *task)
{
    SavePartyMember *member = (SavePartyMember *)(gSaveData + 0x3f8) + task->member;
    Overlay5ObjectSprite *sprites = data_ov007_02090900[task->sprite_index];
    // Unknown stat selectors retain the previously cached value.
    switch (task->stat) {
    case 0:
        task->value = member->experience.fields.level;
        break;
    case 1:
        task->value = member->current_hp;
        break;
    case 2:
        task->value = member->max_hp;
        break;
    case 3:
        task->value = member->power;
        break;
    case 4:
        task->value = member->defense;
        break;
    case 5:
        task->value = member->speed;
        break;
    case 6:
        task->value = member->stache;
        break;
    case 7:
        task->value = member->experience.fields.value;
        break;
    case 8:
        task->value = member->experience_to_next_level.fields.value;
        break;
    }
    func_ov007_0207a680(sprites, task->x, task->y, task->value, task->divisor, task->digits, 1, 2,
                        12, 0);
}
