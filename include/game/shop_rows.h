#ifndef PIT_GAME_SHOP_ROWS_H
#define PIT_GAME_SHOP_ROWS_H
#include <nitro.h>

/* Shared 72-byte task storage: row state or a child segment index. */
typedef struct ShopRowTask {
    u8 unknown_00[16];
    struct ShopRowTask *parent;
    u8 unknown_14[20];
    union {
        int part;
        struct {
            u16 row, value, palette, priority, scale, unknown_32;
            s32 tile, x, y, offset_x, offset_y;
        };
    };
} ShopRowTask;
typedef char ShopRowTaskSizeCheck[sizeof(ShopRowTask) == 72 ? 1 : -1];

typedef struct ShopInventoryRowTask {
    u8 unknown_00[16];
    struct ShopInventoryRowTask *parent;
    u8 unknown_14[20];
    union {
        int part;
        struct {
            u16 row, quantity, value, palette, priority, unknown_32;
            s32 tile, x, y, offset_x, offset_y;
        };
    };
} ShopInventoryRowTask;
typedef char ShopInventoryRowTaskSizeCheck[sizeof(ShopInventoryRowTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void ShopInventoryRow_Update(ShopInventoryRowTask *task);
void ShopInventoryRow_DrawTextSegment(ShopInventoryRowTask *task);
void ShopInventoryRow_DrawInsetSprite(ShopInventoryRowTask *task);
void ShopInventoryRow_DrawQuantityDigit(ShopInventoryRowTask *task);
void ShopInventoryRow_DrawValueSprite(ShopInventoryRowTask *task);
void ShopRow_Update(ShopRowTask *task);
void ShopRow_DrawTextSegment(ShopRowTask *task);
void ShopRow_DrawInsetSprite(ShopRowTask *task);
void ShopRow_DrawValueSprite(ShopRowTask *task);
void ShopRow_DrawAnchorModel(ShopRowTask *task);
void ShopRow_DrawRowModel(ShopRowTask *task);
#ifdef __cplusplus
}
#endif
#endif
