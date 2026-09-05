#include <game/overlay018_attack.h>

enum Overlay18AttackThresholdConstant {
    OVERLAY18_THRESHOLD_ROW_COUNT = 10,
};

extern const s16 *gOverlay18HorizontalDistanceTables[];
extern const s16 *gOverlay18InputTimingWindowTables[];
extern const s16 gOverlay18AlternateActorChanceTable[];
extern const s16 gOverlay18MotionScaleQ8Table[];
extern const s16 gOverlay18TravelSpeedQ8Table[];
extern const s16 gOverlay18LaunchHeightTable[];
extern const s16 gOverlay18MotionDurationByGradeTable[];
extern const s16 gOverlay18TerminalVelocityByGradeTable[];

/* Metrowerks emits C functions in reverse source order. */
const s16 *Overlay18Attack_SelectThresholdRow(
    const s16 *table, int row_width, int value) {
    const s16 *row = table;
    u32 index;

    for (index = 0; index < OVERLAY18_THRESHOLD_ROW_COUNT; ++index) {
        if (value <= row[0]) {
            return &row[1];
        }
        row += row_width;
    }
    return (const s16 *)((const u8 *)table + 18 * row_width + 2);
}

int Overlay18Attack_GetMotionScaleQ8(int progress) {
    return *Overlay18Attack_SelectThresholdRow(
        gOverlay18MotionScaleQ8Table, 2, progress);
}

int Overlay18Attack_GetTravelSpeedQ8(int progress) {
    return *Overlay18Attack_SelectThresholdRow(
        gOverlay18TravelSpeedQ8Table, 2, progress);
}

int Overlay18Attack_GetHorizontalDistance(int progress, int grade) {
    return *Overlay18Attack_SelectThresholdRow(
        gOverlay18HorizontalDistanceTables[grade], 2, progress);
}

const s16 *Overlay18Attack_GetInputTimingWindow(
    int progress, int grade) {
    return Overlay18Attack_SelectThresholdRow(
        gOverlay18InputTimingWindowTables[grade], 3, progress);
}

int Overlay18Attack_GetLaunchHeight(int progress) {
    return *Overlay18Attack_SelectThresholdRow(
        gOverlay18LaunchHeightTable, 2, progress);
}

int Overlay18Attack_GetAlternateActorChance(int progress) {
    return *Overlay18Attack_SelectThresholdRow(
        gOverlay18AlternateActorChanceTable, 2, progress);
}

int Overlay18Attack_GetMotionDuration(int progress, int grade) {
    return Overlay18Attack_SelectThresholdRow(
        gOverlay18MotionDurationByGradeTable, 5, progress)[grade - 1];
}

int Overlay18Attack_GetTerminalVelocityParameter(
    int progress, int grade) {
    return Overlay18Attack_SelectThresholdRow(
        gOverlay18TerminalVelocityByGradeTable, 5, progress)[grade - 1];
}
