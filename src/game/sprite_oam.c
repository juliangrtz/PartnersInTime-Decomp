#include <game/sprite_output.h>

void GameOam_SortGroups(int screen)
{
    int count = data_0205a058[screen];
    int i;
    int swapped;
    int last;
    GameOamSortRecord *groups;
    if (count <= 1) return;
    last = count - 1;
    groups = data_0205a09c[screen];
    while (last >= 0) {
        swapped = -1;
        i = 1;
        if (last >= 1) {
        GameOamSortRecord *group = groups + 1;
        do {
            if (groups[i - 1].key > groups[i].key) {
                u32 key = group->key;
                u32 range = group->range.raw;
                swapped = i - 1;
                group->key = group[-1].key;
                group->range.raw = group[-1].range.raw;
                group[-1].key = key;
                group[-1].range.raw = range;
            }
            ++i;
            ++group;
        } while (i <= last);
        }
        last = swapped;
    }
}

void GameOam_BuildSorted(int screen, const GameOamEntry *source, GameOamEntry *destination)
{
    GameOamEntry *buffer;
    int first;
    GameOamSortRecord *group;
    int i;
    int count;
    first = data_0205a064[screen];
    buffer = data_0205a068;
    if (first) GameSprite_CopyBytes(source, buffer, first * sizeof(*source));
    group = data_0205a09c[screen];
    count = data_0205a058[screen];
    for (i = 0; i < count; ++i) {
        GameSprite_CopyBytes(source + group->range.bytes.first, buffer + first,
                             group->range.bytes.count * sizeof(*source));
        first += group->range.bytes.count;
        ++group;
    }
    if (first != 128) GameSprite_CopyBytes(source + first, buffer + first, (128 - first) * sizeof(*source));
    GameOam_CopyAffine(source, buffer);
    GameSprite_CopyBytes(buffer, destination, 1024);
}
