#ifndef PIT_GAME_FIELD_PALETTE_CROSSFADE_H
#define PIT_GAME_FIELD_PALETTE_CROSSFADE_H
#include <game/field_area.h>
enum FieldPaletteCrossfadeRegion {
    FIELD_PALETTE_OBJ,
    FIELD_PALETTE_OBJ_EXTENDED,
    FIELD_PALETTE_BG,
    FIELD_PALETTE_BG_EXTENDED_0,
    FIELD_PALETTE_BG_EXTENDED_1,
    FIELD_PALETTE_BG_EXTENDED_2,
    FIELD_PALETTE_BG_EXTENDED_3
};
typedef struct FieldPaletteCrossfade {
    struct {
        u32 active : 1, paused : 1, captured : 1, region : 3, source_bank : 4, target_bank : 4,
            unknown_14_31 : 18;
    } flags;
    u16 duration, time;
    u16 colors[256], source[256], target[256];
} FieldPaletteCrossfade;
typedef char FieldPaletteCrossfade_SizeCheck[sizeof(FieldPaletteCrossfade) == 1544 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void FieldPaletteCrossfade_Start(FieldAreaContext *field, int region, int source_bank, int target_bank,
                                 int duration);
void FieldPaletteCrossfade_Transfer(FieldAreaContext *field);
#ifdef __cplusplus
}
#endif
#endif
