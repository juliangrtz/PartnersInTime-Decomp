#include <game/overlay018_attack.h>

extern s16 data_ov018_020c7400[][3], data_ov018_020c7478[][3];
extern s16 data_ov018_020c752c[][3], data_ov018_020c74b4[][3];
extern s16 data_ov018_020c743c[][3], data_ov018_020c74f0[][3];
extern s16 data_ov018_020c72e8[][2], gOverlay18MotionScaleQ8Table[][2];
extern s16 gOverlay18TravelSpeedQ8Table[][2], data_ov018_020c7388[][2], data_ov018_020c73d8[][2];

/* Adds the per-row adjustments selected by the badge-dependent setup path. */
void Overlay18Attack_ApplyTableDeltas(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        int delta = data_ov018_020c72e8[i][1];
        gOverlay18MotionScaleQ8Table[i][1] += delta;
        gOverlay18TravelSpeedQ8Table[i][1] += delta;
        data_ov018_020c7388[i][1] += data_ov018_020c73d8[i][1];
        data_ov018_020c7478[i][1] += data_ov018_020c7400[i][1];
        data_ov018_020c7478[i][2] += data_ov018_020c7400[i][2];
        data_ov018_020c74b4[i][1] += data_ov018_020c752c[i][1];
        data_ov018_020c74b4[i][2] += data_ov018_020c752c[i][2];
        data_ov018_020c74f0[i][1] += data_ov018_020c743c[i][1];
        data_ov018_020c74f0[i][2] += data_ov018_020c743c[i][2];
    }
}
