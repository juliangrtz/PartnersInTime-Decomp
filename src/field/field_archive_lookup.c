#include <game/field_archive.h>
#include <game/save_data.h>

extern const char *data_ov000_020c0f30[];
extern u32 data_ov000_020c0624[];
extern u32 data_ov000_020c063c[];

const char *FieldArchive_GetDescriptor(FieldArchive *archive, int index)
{
    return data_ov000_020c0f30[index];
}

FieldArchiveRecord *FieldArchive_GetPrimaryRecord(FieldArchive *archive, u32 id)
{
    u32 category = id & 0xFF000000;
    u32 entry = id & 0xFFFFFF;
    int i;
    for (i = 0; i < 3; ++i) {
        if (category == data_ov000_020c0624[2 * i]) {
            FieldArchiveRecord *record = archive->first_records[data_ov000_020c0624[2 * i + 1]] + entry;
            if (record->localized)
                record += ((SaveLocalizedResourceState *)gSaveData)->language;
            return record;
        }
    }
    return 0;
}

FieldArchiveRecord *FieldArchive_GetSecondaryRecord(FieldArchive *archive, u32 id)
{
    u32 category = id & 0xFF000000;
    u32 entry = id & 0xFFFFFF;
    int i;
    for (i = 0; i < 3; ++i) {
        if (category == data_ov000_020c0624[2 * i])
            return archive->second_records[data_ov000_020c0624[2 * i + 1]] + entry;
    }
    return 0;
}

int FieldArchive_GetArchiveIndex(FieldArchive *archive, u32 id)
{
    u32 category = id & 0xFF000000;
    int i;
    for (i = 0; i < 3; ++i) {
        if (category == data_ov000_020c063c[2 * i])
            return data_ov000_020c063c[2 * i + 1];
    }
    return 0;
}
