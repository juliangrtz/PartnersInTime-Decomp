#include <game/battle_mesh.h>
#include <game/battle_scene.h>
#include <game/vector_math.h>

extern const s16 FX_SinCosTable_[];
void MTX_ApplyRotZ44(int cosine, int sine, s32 *first, s32 *second);
void func_02016334(s32 *vector, int scale);

void BattleMesh_RotateAndNormalize(BattleSceneObject *object, int axis, int angle, int scale)
{
    BattleMeshResource *resource = (BattleMeshResource *)object->resource->data;
    s32 *matrix = (s32 *)&resource->transform;
    if (angle) {
        int cosine = FX_SinCosTable_[(angle >> 4) * 2 + 1];
        int sine = FX_SinCosTable_[(angle >> 4) * 2];
        switch (axis) {
        case 0:
            MTX_ApplyRotZ44(cosine, sine, matrix + 1, matrix + 2);
            break;
        case 1:
            MTX_ApplyRotZ44(cosine, sine, matrix + 2, matrix);
            break;
        case 2:
            MTX_ApplyRotZ44(cosine, sine, matrix, matrix + 1);
            break;
        }
    }
    func_02016334(matrix, scale);
    GameVector_CrossProduct(matrix + 4, matrix + 8, matrix, 0);
    func_02016334(matrix + 4, scale);
    GameVector_CrossProduct(matrix + 8, matrix, matrix + 4, 0);
    func_02016334(matrix + 8, scale);
}
