#include <game/battle_mesh.h>

void BattleMesh_PrepareTriangles(BattleMeshTriangle *triangles, int count, const MtxFx44 *matrix, u16 color,
                                 s16 alpha)
{
    BattleMeshTriangle *triangle = triangles;
    int i;
    for (i = 0; i < count; ++i, ++triangle) {
        const BattleMeshVertex *a, *b, *c;
        BattleMeshVertex *vertices = triangle->vertices;
        BattleMeshCenter *center = &triangle->center;
        triangle->color = color;
        triangle->alpha = alpha;
        a = triangle->source[0];
        b = triangle->source[1];
        c = triangle->source[2];
        center->x = (a->x + b->x + c->x) / 3;
        vertices[0].x = a->x - center->x;
        vertices[1].x = b->x - center->x;
        vertices[2].x = c->x - center->x;
        center->y = (a->y + b->y + c->y) / 3;
        vertices[0].y = a->y - center->y;
        vertices[1].y = b->y - center->y;
        vertices[2].y = c->y - center->y;
        center->z = (a->z + b->z + c->z) / 3;
        vertices[0].z = a->z - center->z;
        vertices[1].z = b->z - center->z;
        vertices[2].z = c->z - center->z;
        if (triangle->update)
            triangle->update(triangle);
        BattleMesh_TransformVertex(&vertices[0], matrix);
        BattleMesh_TransformVertex(&vertices[1], matrix);
        BattleMesh_TransformVertex(&vertices[2], matrix);
        BattleMesh_TransformCenter(center, matrix);
    }
}

BattleMeshVertex *BattleMesh_TransformVertex(BattleMeshVertex *vertex, const MtxFx44 *matrix)
{
    s16 x = vertex->x, y = vertex->y, z = vertex->z;
    vertex->x = x * matrix->_00 / 4096 + y * matrix->_10 / 4096 + z * matrix->_20 / 4096;
    vertex->y = x * matrix->_01 / 4096 + y * matrix->_11 / 4096 + z * matrix->_21 / 4096;
    vertex->z = x * matrix->_02 / 4096 + y * matrix->_12 / 4096 + z * matrix->_22 / 4096;
    return vertex;
}

/* Divide each Q12 product separately, preserving native truncation toward zero. */
BattleMeshCenter *BattleMesh_TransformCenter(BattleMeshCenter *center, const MtxFx44 *matrix)
{
    int x = center->x, y = center->y, z = center->z;
    center->x = x * matrix->_00 / 4096 + y * matrix->_10 / 4096 + z * matrix->_20 / 4096;
    center->y = x * matrix->_01 / 4096 + y * matrix->_11 / 4096 + z * matrix->_21 / 4096;
    center->z = x * matrix->_02 / 4096 + y * matrix->_12 / 4096 + z * matrix->_22 / 4096;
    return center;
}

/* Link visible triangles from far to near; newer entries precede equal depths. */
BattleMeshTriangle *BattleMesh_SortTriangles(BattleMeshTriangle *triangles, int count, int cull_backfaces)
{
    BattleMeshTriangle *first = 0;
    for (; count > 0; --count, ++triangles) {
        BattleMeshTriangle *previous, *current;
        int depth;
        if (!cull_backfaces || (triangles->vertices[1].x - triangles->vertices[0].x) *
                                       (triangles->vertices[2].y - triangles->vertices[0].y) >
                                   (triangles->vertices[2].x - triangles->vertices[0].x) *
                                       (triangles->vertices[1].y - triangles->vertices[0].y)) {
            previous = 0;
            depth = triangles->center.z;
            for (current = first; current && depth < current->center.z; current = current->next)
                previous = current;
            triangles->next = current;
            if (previous)
                previous->next = triangles;
            else
                first = triangles;
        }
    }
    return first;
}
