/*
 * Field projection (overlay 0, 0x02068D24-0x02068E04).
 *
 * The projection registers the field draws its 3D content through.
 */

#include <game/field_system.h>
#include <nitro/gx_projection.h>
extern void func_02037108(void);
#define REG32(a) (*(vu32 *)(a))
void FieldSystem_LoadProjection(FieldSystem *field) {
    func_02037108();
    REG32(0x4000440) = 0;
    REG32(0x4000454) = 0;
    G3i_PerspectiveW(2048, 3547, 5461, 4096, 0x190000, 4096, 1, 0);
    REG32(0x400044c) = 0;
    G3i_LookAt(&field->camera_position, &field->camera_up, &field->camera_target, 1, 0);
    REG32(0x4000440) = 3;
    REG32(0x4000454) = 0;
    REG32(0x4000440) = 2;
    REG32(0x400046c) = 4096;
    REG32(0x400046c) = 4096;
    REG32(0x400046c) = 4096;
    REG32(0x40004a8) = 0x41b00000;
}
