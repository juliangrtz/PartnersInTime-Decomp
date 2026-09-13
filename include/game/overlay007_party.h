#ifndef PIT_GAME_OVERLAY007_PARTY_H
#define PIT_GAME_OVERLAY007_PARTY_H

#include <nitro.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <game/text.h>

/* The scene's party roster: the rotating display order, the per-slot values the
   interface shows and the text object the captions are measured with. */
typedef struct Overlay7Party {
    union { u8 unknown_00; u8 visible; };
    u8 kind;
    u8 unknown_02[2];
    u8 slots[128];
    s8 *values;
    u8 unknown_88[128];
    s8 first;
    u8 count;
    union { u8 unknown_10a; s8 tile_row; };
    union { s8 offset; u8 index; } span;
    union {
        /* Preserve the existing view used by the rotating-order helpers. */
        struct { s8 offsets[4]; u8 unknown_110[12]; };
        struct {
            s8 saved_first[5], saved_selected[5];
            u8 unknown_116[2];
            void *list_images;
        };
    };
    GameText text;
} Overlay7Party;

/* Only the part modelled above is pinned; the roster may carry further fields. */
typedef char Overlay7PartySizeCheck[sizeof(Overlay7Party) == 0x14c ? 1 : -1];

/* One row of the item table: the identifiers, the packed price word and the
   five stat deltas the gear rows contribute. */
typedef struct Overlay7Item {
    u16 id;
    union { u16 unknown_02; u16 name_id; };
    union { u16 unknown_04; u16 description_id; };
    u16 unknown_06;
    u8 unknown_08[4];
    union {
        u32 raw;
        struct { u32 amount : 16, percent : 8, group : 1, reserved : 7; } bits;
    } packed;
    s16 hp, power, defense, speed, stache;
    u16 unknown_1a;
} Overlay7Item;

typedef char Overlay7ItemSizeCheck[sizeof(Overlay7Item) == 0x1c ? 1 : -1];

extern const Overlay7Item data_020505c4[];

u16 PauseList_MeasureRowWidth(Overlay7Party *party, int row, int plural);
void PauseList_Hide(Overlay7Party *party);
void PauseList_Show(Overlay7Party *party);
int PauseList_GetTileRow(Overlay7Party *party);
u16 PauseList_DrawSelectedLabel(Overlay7Party *party);
void PauseList_RedrawSelectedRow(Overlay7Party *party, int copy);
u32 PauseList_CopySelectedRow(Overlay7Party *party);
int PauseList_CheckRowAvailability(Overlay7Party *party, int row, int member, int allow_other);
u16 PauseItem_GetDescriptionId(Overlay7Party *party, int kind, u16 item);
u16 PauseItem_GetNameId(Overlay7Party *party, int kind, u16 item);
int PauseItem_CheckRotatedAvailability(Overlay7Party *party, int kind, u32 member, int row, u16 count);
int PauseItem_RebuildRotatingOrder(Overlay7Party *party, int kind);

#ifdef __cplusplus
}
#endif

#endif
