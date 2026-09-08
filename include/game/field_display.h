#ifndef PIT_GAME_FIELD_DISPLAY_H
#define PIT_GAME_FIELD_DISPLAY_H
#include <nitro.h>
/* Standard BG/OBJ palettes, BG extended slots 1-3, and all OBJ extended palettes. */
typedef struct FieldPaletteBackup {
    u8 background[512];
    u8 objects[512];
    u8 extended_background[3][512];
    u8 extended_objects[8192];
} FieldPaletteBackup;
typedef char FieldPaletteBackup_SizeCheck[sizeof(FieldPaletteBackup) == 10752 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldDisplay_InitializeGeometry(void *unused);
void FieldDisplay_InitializeVram(void *unused);
void FieldDisplay_SavePalettes(void);
#ifdef __cplusplus
}
#endif
#endif
