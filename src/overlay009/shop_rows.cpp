#include <game/battle_scene.h>
#include <game/shop_list.h>
#include <game/shop_panels.h>
#include <game/shop_rows.h>

/* The first 64 bytes of the attached sprite, through its affine matrix. */
struct ShopRowSprite {
    u8 unknown_00[20];
    u32 attributes_0;
    union {
        u32 attributes_1;
        struct {
            u32 tile : 10, size : 2, palette : 4, unknown_16_31 : 16;
        } bits;
    };
    s32 x, y, offset_x, offset_y;
    s16 origin_x, origin_y;
    u8 unknown_30[8];
    s16 matrix[4];
};
typedef char ShopRowSpriteSizeCheck[sizeof(ShopRowSprite) == 64 ? 1 : -1];
class ShopRowPanelInterface
{
  public:
    virtual void destroy();
    virtual void deleteInstance();
    virtual u32 get_row_value(u16 row);
};
extern "C" {
extern ShopItemPanel *data_ov009_0207ea34;
extern ShopList *data_ov009_0207ea2c;
extern u8 data_ov005_0206a180[];
BattleModel *Overlay5ResourceA_Get(ShopRowTask *);
ShopRowSprite *Overlay5ResourceB_Get(ShopRowTask *);
void func_ov005_020692c0(ShopRowSprite *, u16, u16, u16);
void func_ov005_02069084(void *, u8);
int func_ov009_0207171c(ShopItemPanel *, int);
int func_ov009_02071748(ShopItemPanel *, int);
void func_ov009_0207c8f0(int, int, u16, int);
static inline void SetModelPosition(BattleModel *model, int x, int y)
{
    model->animation_offset_x = x;
    model->animation_offset_y = y;
}

void ShopRow_Update(ShopRowTask *task)
{
    int first = ShopItemPanel_GetRingFirst(data_ov009_0207ea34);
    int row = task->row;
    int slot = (first + row) % ShopItemPanel_GetVisibleRows(data_ov009_0207ea34);
    int old_value;
    task->x = task->offset_x + ShopList_GetRowX(data_ov009_0207ea2c, (u8)row);
    task->y = task->offset_y + ShopList_GetRowY(data_ov009_0207ea2c, (u8)task->row);
    task->priority = ShopList_GetRowPriority(data_ov009_0207ea2c, (u8)task->row);
    task->scale = ShopList_GetScale(data_ov009_0207ea2c);
    task->tile = (36 * slot + 76) / 4;
    old_value = task->value;
    task->value = ((ShopRowPanelInterface *)data_ov009_0207ea34)->get_row_value(task->row);
    if (old_value != task->value)
        func_ov009_0207c8f0(16 * task->row + 520, 30, task->value, 4);
    if (func_ov009_02071748(data_ov009_0207ea34, task->row))
        task->palette = 0;
    else
        task->palette = 1;
}
void ShopRow_DrawTextSegment(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int part = task->part;
    ShopRowTask *row = task->parent;
    int y, top, bottom;
    sprite->bits.tile = row->tile + 8 * part / 4;
    sprite->bits.palette = (data_ov005_0206a180[0x71] + row->palette) & 15;
    sprite->x = row->x;
    sprite->y = row->y;
    if (row->scale > 409) {
        func_ov005_020692c0(sprite, 256, row->scale >> 4, 0);
        y = (sprite->y + sprite->offset_y) / 4096 - sprite->origin_y;
        ShopItemPanel_GetVector334(data_ov009_0207ea34, &top, &bottom);
        if (top >= y || bottom <= y + 12)
            func_ov005_02069084(sprite, (u8)row->priority);
    }
}
void ShopRow_DrawInsetSprite(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopRowTask *row = task->parent;
    int y, top, bottom;
    sprite->bits.tile = row->tile + 8;
    sprite->x = row->x;
    sprite->y = row->y;
    if (row->scale > 409) {
        func_ov005_020692c0(sprite, 256, row->scale >> 4, 0);
        y = (sprite->y + sprite->offset_y) / 4096 - sprite->origin_y;
        ShopItemPanel_GetVector334(data_ov009_0207ea34, &top, &bottom);
        if (top >= y || bottom <= y + 8)
            func_ov005_02069084(sprite, (u8)row->priority);
    }
}
void ShopRow_DrawValueSprite(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopRowTask *row = task->parent;
    sprite->x = row->x;
    sprite->y = row->y;
    if (row->scale > 409) {
        func_ov005_020692c0(sprite, 256, row->scale >> 4, 0);
        func_ov005_02069084(sprite, (u8)row->priority);
    }
}
void ShopRow_DrawAnchorModel(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    int x = ShopList_GetPointX(data_ov009_0207ea2c, 3) / 4096 - 48;
    int y = ShopList_GetPointY(data_ov009_0207ea2c, 3) / 4096 + 8;
    int scale = ShopList_GetScale(data_ov009_0207ea2c);
    if (scale > 409) {
        model->scale_x = 256;
        model->scale_y = scale / 16;
        model->animation_offset_x = x;
        model->animation_offset_y = y;
        func_ov005_02069084(model, 5);
    }
}
void ShopRow_DrawRowModel(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    ShopRowTask *row = task->parent;
    if (func_ov009_0207171c(data_ov009_0207ea34, row->row)) {
        u32 scale = row->scale;
        if (scale > 409) {
            model->scale_x = 256;
            model->scale_y = scale >> 4;
            SetModelPosition(model, row->x / 4096, row->y / 4096);
            func_ov005_02069084(model, (u8)row->priority);
        }
    }
}
}
