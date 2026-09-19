#ifndef PIT_SCENE_OBJECT_INTERNAL_H
#define PIT_SCENE_OBJECT_INTERNAL_H
#include <game/battle_scene.h>
extern "C" {
#include <game/scene_object.h>
extern u8 *data_ov007_020a6b90;
extern u8 data_ov005_0206a180[];
extern void *func_ov005_020698dc(int);
extern void func_ov005_02068908(BattleModel *, int, void *, int, int);
extern void *func_ov005_02068c80(void);
extern int func_ov005_02067248(void *, void *);
extern void *func_ov005_02066a7c(void *, u8, u16, void *, int, int),
    *func_ov005_02066920(void *, u8, u16, void *, int, int);
extern void func_ov005_020698c8(u32, void *);
}
#define ARCHIVE (*(void **)(data_ov005_0206a180 + 0x2c))
#define PRIMARY(o) ((BattleModel *)(o)->primary_renderable)
#define SECONDARY(o) ((BattleModel *)(o)->secondary_renderable)
#define META(i) (((void **)(data_ov007_020a6b90 + 13884))[i])
#define SLOTS(i) (*(SceneObject **)(data_ov007_020a6b90 + 12892 + 4 * (i)))
#define OBJECT(id) ((SceneObject *)SceneObject_GetById(id))
#endif
