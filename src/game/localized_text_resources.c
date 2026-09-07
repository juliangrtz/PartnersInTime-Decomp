#include <game/save_data.h>
#include <game/text.h>

extern const char *data_0205519c[14];
extern void *GameResource_ReadFile(const char *path);

void GameTextResources_LoadAll(void)
{
    int index;
    for (index = 0; index < 14; ++index) {
        const u8 *data = GameResource_ReadFile(data_0205519c[index]);
        ((SaveLocalizedResourceState *)gSaveData)->tables[index] = data;
    }
}

const void *GameTextResources_GetEntry(int table, int entry)
{
    const u8 *data = ((SaveLocalizedResourceState *)gSaveData)->tables[table];
    data += ((const u32 *)data)[((SaveLocalizedResourceState *)gSaveData)->language] & ~3;
    return data + ((const u32 *)data)[entry];
}
