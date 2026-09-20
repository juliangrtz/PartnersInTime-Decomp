/*
 * Selected key-item model (overlay 7, 0x02073754-0x02073994).
 *
 * Loads the selected item asynchronously, attaches its model once ready, and
 * closes the archive table when the list is hidden and its read has finished.
 */
#include "pause_scene_internal.h"
#include <game/overlay007_party.h>
#include <game/battle_scene.h>
struct KeyItemModelTask {
    u8 unknown_00[32];
    int phase, unknown_24, item;
    u8 unknown_2c[28];
};
typedef char TaskSize[sizeof(KeyItemModelTask) == 72 ? 1 : -1];
// Halfword aliases within the ten-byte key-item records.
struct KeyItemHalfword {
    u16 value;
    u8 remaining[8];
};
extern "C" {
BattleModel *Overlay5ResourceA_Get(void *);
void Overlay5ResourceA_ReleaseCallback(void *);
BattleModel *Overlay5ResourceA_Attach(void *, BattleModel *, int);
Overlay5Asset *func_ov005_020698dc(int);
void func_ov005_020698f0(int);
void func_ov005_020698c8(u32, Overlay5Asset *);
Overlay5Asset *func_ov005_02066a7c(Overlay5Archive *, u16, u16, const void *, u8, int);
int func_ov005_02067248(Overlay5Archive *, u32);
void func_ov005_0206650c(void *);
void func_ov005_02068908(BattleModel *, int, Overlay5Asset *, int, int);
void func_ov005_02069084(void *, int);
u8 func_ov007_02075408(Overlay7Party *);
u8 func_ov007_02075324(Overlay7Party *, int);
extern const KeyItemHalfword data_ov007_0208e02e[];
extern const s8 data_ov007_0208e034[], data_ov007_0208e035[];
static inline void SetPosition(BattleModel *sprite, int x, int y)
{
    sprite->animation_offset_x = x;
    sprite->animation_offset_y = y;
}
void PauseKeyItem_UpdateModel(KeyItemModelTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    if (!((Overlay7Party *)data_ov007_0208e1e4)->visible) {
        if (func_ov005_02067248(ARCHIVE, func_ov005_020698dc(84)->key)) {
            func_ov005_02067424(ARCHIVE, 3);
            func_ov005_0206650c(task);
        }
        return;
    }
    int item = func_ov007_02075324((Overlay7Party *)data_ov007_0208e1e4,
                                   func_ov007_02075408((Overlay7Party *)data_ov007_0208e1e4));
    switch (task->phase) {
    case 0:
        if (task->item != item) {
            Overlay5ResourceA_ReleaseCallback(task);
            func_ov005_020698f0(84);
            Overlay5Asset *asset = func_ov005_02066a7c(
                ARCHIVE, 3, ((const KeyItemHalfword *)data_ov007_0208e02c)[item].value,
                ((Overlay7Party *)data_ov007_0208e1e4)->list_images, 0, 0);
            func_ov005_020698c8(84, asset);
            task->item = item;
            ++task->phase;
        }
        break;
    case 1:
        if (func_ov005_02067248(ARCHIVE, func_ov005_020698dc(84)->key)) {
            if (task->item == item) {
                sprite = Overlay5ResourceA_Attach(task, 0, 1);
                func_ov005_02068908(sprite, 0, func_ov005_020698dc(84), 0, -1);
                sprite->set_primary_animation((u8)data_ov007_0208e02e[item].value, 0, 1);
                sprite->flags = (sprite->flags & ~3) | 2;
            }
            task->phase = 0;
        }
        break;
    }
    // Keep the native cached pointer through a same-frame resource release.
    if (sprite) {
        SetPosition(sprite, WORK.label_origin_x + 128 + data_ov007_0208e034[10 * item],
                    WORK.label_origin_y + 136 + data_ov007_0208e035[10 * item]);
        func_ov005_02069084(sprite, 42);
    }
}
}
