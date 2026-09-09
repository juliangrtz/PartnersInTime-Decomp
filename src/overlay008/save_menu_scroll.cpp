#include <game/battle_scene.h>
#include <game/save_menu.h>

/* Prefix through the horizontal position used by the text segment callback. */
struct SaveMenuScrollSpritePrefix {
    u8 unknown_00[28];
    s32 x;
};
typedef char SaveMenuScrollSpritePrefixSizeCheck[sizeof(SaveMenuScrollSpritePrefix) == 32 ? 1 : -1];
extern "C" {
extern u8 data_ov008_02078290[], data_ov008_0207aa64[];
extern u16 data_0206032c;
extern s16 FX_SinCosTable_[];
BattleModel *Overlay5ResourceA_Get(SaveMenuScrollTask *);
SaveMenuScrollSpritePrefix *Overlay5ResourceB_Get(SaveMenuScrollTask *);
void func_ov005_02069084(void *, u8);
void func_ov008_02071400(int);
static inline int ArrowPulse(void)
{
    int angle = (*(u32 *)(data_ov008_02078290 + 0x194) << 11) & 65535;
    return 2 * FX_SinCosTable_[2 * (angle >> 4)] - 8192;
}
void SaveMenuScroll_Update(SaveMenuScrollTask *task)
{
    if (data_ov008_02078290[0x198])
        return;
    if (task->source != data_ov008_0207aa64[((s8 *)data_ov008_02078290)[0x19d]]) {
        func_ov008_02071400(((s8 *)data_ov008_02078290)[0x19d]);
        return;
    }
    switch (task->state) {
    case 0:
        if (task->counter) {
            if (data_0206032c & 0x300)
                task->counter = 0;
            else
                task->counter--;
        } else
            task->state++;
        break;
    case 1: {
        int direction = 0;
        if (data_0206032c & 0x100)
            direction++;
        if (data_0206032c & 0x200)
            direction--;
        task->direction = direction;
        if (direction > 0) {
            task->x += 8192;
            if (task->x / 4096 > 256)
                task->x = (-task->width) << 12;
        } else {
            if (direction < 0)
                task->x -= 8192;
            else
                task->x -= 2560;
            if (task->x / 4096 < -task->width)
                task->x = 256 * 4096;
        }
        break;
    }
    }
}
void SaveMenuScroll_DrawTextSegment(SaveMenuScrollTask *task)
{
    SaveMenuScrollSpritePrefix *sprite = Overlay5ResourceB_Get(task);
    sprite->x = task->parent->x;
    func_ov005_02069084(sprite, 60);
}
void SaveMenuScroll_DrawArrow(SaveMenuScrollTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    SaveMenuScrollTask *parent = task->parent;
    int x = task->anchor_x, y = task->anchor_y;
    if (!data_ov008_02078290[0x198]) {
        if (!task->part) {
            if (parent->direction == -1)
                x -= 16384;
            else
                x += ArrowPulse();
        } else {
            if (parent->direction == 1)
                x += 16384;
            else
                x -= ArrowPulse();
        }
    }
    model->animation_offset_x = x / 4096;
    model->animation_offset_y = y / 4096;
    func_ov005_02069084(model, 59);
}
}
