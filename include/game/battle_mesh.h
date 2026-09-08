#ifndef PIT_GAME_BATTLE_MESH_H
#define PIT_GAME_BATTLE_MESH_H

#include <nitro/fx_mtx.h>

typedef struct BattleMeshVertex {
    s16 x, y, z;
} BattleMeshVertex;
typedef struct BattleMeshCenter {
    s32 x, y, z;
} BattleMeshCenter;
typedef struct BattleMeshTriangle {
    struct BattleMeshTriangle *next;
    const BattleMeshVertex *source[3];
    BattleMeshVertex vertices[3];
    u8 unknown_22[2];
    BattleMeshCenter center;
    u16 color;
    s16 alpha;
    void (*update)(struct BattleMeshTriangle *);
    u8 unknown_38[8];
} BattleMeshTriangle;
typedef char BattleMeshTriangle_SizeCheck[sizeof(BattleMeshTriangle) == 64 ? 1 : -1];

typedef char BattleMeshVertex_SizeCheck[sizeof(BattleMeshVertex) == 6 ? 1 : -1];
typedef char BattleMeshCenter_SizeCheck[sizeof(BattleMeshCenter) == 12 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

BattleMeshCenter *BattleMesh_TransformCenter(BattleMeshCenter *center, const MtxFx44 *matrix);
BattleMeshVertex *BattleMesh_TransformVertex(BattleMeshVertex *vertex, const MtxFx44 *matrix);
BattleMeshTriangle *BattleMesh_SortTriangles(BattleMeshTriangle *triangles, int count, int cull_backfaces);
void BattleMesh_PrepareTriangles(BattleMeshTriangle *triangles, int count, const MtxFx44 *matrix, u16 color,
                                 s16 alpha);

#ifdef __cplusplus
}
#endif
#endif
