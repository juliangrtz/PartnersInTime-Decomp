/* Resolve motion from the engine angle table. X/Y divisions truncate toward
 * zero at each stage; the spatial Z output deliberately retains the raw product.
 * The entity parameter is unused, as in the original member-style interface. */
#include <game/field_entity_motion.h>
extern "C" const s16 FX_SinCosTable_[];
extern "C" void FieldEntity3D_ResolveDirectionalMotion(FieldRuntimeEntity *entity, fx32 speed,
                                                       int planar, int direction, u16 elevation,
                                                       fx32 *x, fx32 *y, fx32 *z)
{
    if (planar) {
        if (x)
            *x = speed * FX_SinCosTable_[2 * (direction >> 4)] / 4096;
        if (y)
            *y = -speed * FX_SinCosTable_[2 * (direction >> 4) + 1] / 4096;
        if (z)
            *z = 0;
    } else {
        if (x)
            *x = FX_SinCosTable_[2 * (elevation >> 4) + 1] *
                 (speed * FX_SinCosTable_[2 * (direction >> 4)] / 4096) / 4096;
        if (y)
            *y = FX_SinCosTable_[2 * (elevation >> 4) + 1] *
                 (-speed * FX_SinCosTable_[2 * (direction >> 4) + 1] / 4096) / 4096;
        if (z)
            *z = speed * FX_SinCosTable_[2 * (elevation >> 4)];
    }
}
