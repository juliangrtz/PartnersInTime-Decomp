#include <game/palette_animation.h>

const s16 *GameAnimationTable_GetDefaultTrack(const s16 *table, int animation)
{
    int index = animation + 3;
    int offset;
    if (index < 2)
        return 0;
    if (table[0] <= index)
        index = 2;
    offset = table[index];
    if (!offset)
        offset = table[2];
    if (!offset) return 0;
    return table + offset;
}

const s16 *GameAnimationTable_GetTrack(const s16 *table, int animation)
{
    int index = animation + table[0];
    int offset;
    if (index < 2)
        return 0;
    if (table[1] <= index)
        return 0;
    offset = table[index];
    if (offset == 0)
        table = 0;
    else
        table += offset;
    return table;
}

const s16 *GameAnimationTable_GetComponent(const s16 *table, int component, int animation)
{
    int index;
    if (component < -1)
        return 0;
    if (component >= 4)
        return 0;
    if (animation < 0)
        return GameAnimationTable_GetDefaultTrack(table, component);
    index = animation * 4;
    if (component >= 0)
        index += component;
    return GameAnimationTable_GetTrack(table, index);
}
