#ifndef PIT_GAME_SHOP_LIST_H
#define PIT_GAME_SHOP_LIST_H
#include <nitro.h>
typedef struct ShopListPoint {
    s32 x, y, previous_x, previous_y, velocity_x, velocity_y;
    u16 gravity_scale;
    u8 pinned, delay;
} ShopListPoint;
typedef struct ShopListLink {
    ShopListPoint *first, *second;
    s32 length;
} ShopListLink;
typedef struct ShopList {
    u8 visible, moving, entering, settled_frames, point_count, unknown_05;
    u16 phase, frames, unknown_0a;
    ShopListPoint points[16];
    ShopListLink links[15];
    s32 anchor_x, anchor_y;
    u16 priority, unknown_28a;
    s32 scale;
} ShopList;
typedef char ShopListSizeCheck[sizeof(ShopList) == 656 ? 1 : -1];
typedef char ShopListPointSizeCheck[sizeof(ShopListPoint) == 28 ? 1 : -1];
typedef struct ShopListScaleTask ShopListScaleTask;
typedef struct ShopListPointTask ShopListPointTask;

#ifdef __cplusplus
extern "C" {
#endif
u16 ShopList_GetPhase(ShopList *list);
void ShopList_AnimateScale(s32 initial, s32 target, int frames, int delay);
void ShopListScale_Update(ShopListScaleTask *task);
void ShopListPoint_DrawModel(ShopListPointTask *task);
void ShopList_Simulate(ShopList *list, int iterations);
void ShopList_RestorePinnedPoints(ShopList *list, ShopListPoint *point, int count);
void ShopList_UpdateVelocities(ShopList *list, ShopListPoint *point, int count);
void ShopList_AdvancePoints(ShopList *list, ShopListPoint *point, int count);
s32 ShopList_GetScale(ShopList *list);
void ShopList_SetScale(ShopList *list, s32 scale);
int ShopList_GetRowPriority(ShopList *list, u8 row);
s32 ShopList_GetRowY(ShopList *list, u8 row);
s32 ShopList_GetRowX(ShopList *list, u8 row);
int ShopList_GetPointPriority(ShopList *list, u8 index);
s32 ShopList_GetPointY(ShopList *list, u8 index);
s32 ShopList_GetPointX(ShopList *list, u8 index);
void ShopList_BeginExit(ShopList *list);
void ShopList_Destroy(ShopList *list);
ShopList *ShopList_Init(ShopList *list);
int ShopInventory_CanEquip(u16 item, u32 member);
void ShopInventory_Add(u16 item, s8 delta);
s8 ShopInventory_GetCount(u16 item, u32 mode);
s8 *ShopInventory_GetArray(u16 item);
#ifdef __cplusplus
}
#endif
#endif
