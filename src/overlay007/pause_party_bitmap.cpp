#include "pause_scene_internal.h"
#include <game/pause_party_bitmap.h>

/* Prefix of the live save context through its four existing member records. */
struct PausePartySaveView {
    u8 unknown_000[1016];
    SavePartyMember members[4];
};
typedef char PausePartySaveViewSize[sizeof(PausePartySaveView) == 1160 ? 1 : -1];

extern "C" {
extern u8 data_ov007_0208db50[6];
void func_ov007_0207952c(int, int, int, int, int, int, int, int);
void MIi_CpuClear16(u16, void *, u32);
}

extern "C" void PausePartyBitmap_DrawValue(int x, int y, int member, int kind, int digits, int leading_zeroes)
{
    int value;
    switch (kind) {
    case PAUSE_PARTY_VALUE_LEVEL:
        value = ((PausePartySaveView *)gSaveData)->members[member].experience.fields.level;
        break;
    case PAUSE_PARTY_VALUE_CURRENT_HP:
        value = ((PausePartySaveView *)gSaveData)->members[member].current_hp;
        break;
    case PAUSE_PARTY_VALUE_MAX_HP:
        value = ((PausePartySaveView *)gSaveData)->members[member].max_hp;
        break;
    }
    int divisor = 1;
    for (int digit = 0; digit < digits - 1; ++digit)
        divisor *= 10;
    if (value >= 10 * divisor)
        value = 10 * divisor - 1;
    for (int digit = 0; digit < digits; ++digit) {
        int number = value / divisor;
        if (number || divisor == 1 || leading_zeroes) {
            func_ov007_0207952c(member, x, y, 8 * number + 88, 2, 2, 32, 1);
            leading_zeroes = 1;
        }
        x += 8;
        value %= divisor;
        divisor /= 10;
    }
}

extern "C" void PausePartyBitmap_Rebuild(int member)
{
    {
        // Preserve the native clear wrapper's stack halfword store/load.
        volatile u16 zero = 0;
        MIi_CpuClear16(zero, (u8 *)WORK.owned8c + 4480 * member, 4480);
    }
    int level_x = data_ov007_0208db50[0];
    int level_y = data_ov007_0208db50[1];
    func_ov007_0207952c(member, level_x - 32, level_y + 7, 248, 4, 1, 48, 0);
    PausePartyBitmap_DrawValue(level_x, level_y, member, PAUSE_PARTY_VALUE_LEVEL, 3, 0);
    int hp_x = data_ov007_0208db50[2];
    int hp_y = data_ov007_0208db50[3];
    func_ov007_0207952c(member, hp_x - 30, hp_y + 7, 256, 4, 1, 48, 0);
    PausePartyBitmap_DrawValue(hp_x, hp_y, member, PAUSE_PARTY_VALUE_CURRENT_HP, 3, 0);
    int maximum_x = data_ov007_0208db50[4];
    int maximum_y = data_ov007_0208db50[5];
    func_ov007_0207952c(member, maximum_x - 7, maximum_y, 264, 1, 2, 32, 1);
    PausePartyBitmap_DrawValue(maximum_x, maximum_y, member, PAUSE_PARTY_VALUE_MAX_HP, 3, 0);
}
