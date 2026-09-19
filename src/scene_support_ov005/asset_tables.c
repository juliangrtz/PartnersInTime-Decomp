#include <game/overlay005_archive.h>
#include <game/save_data.h>
#include <game/heap.h>

/* One asset row: the image and palette entries plus a flag that selects the
   alternate row for the current save. */
typedef struct Overlay5AssetRow {
    u16 image;
    u16 unknown_02;
    u8 unknown_04[3];
    union { u8 raw; struct { u8 alternate : 1, reserved1 : 7; } bits; } flags;
} Overlay5AssetRow;

typedef struct Overlay5RowTable {
    u32 count;
    u32 reserved04;
    Overlay5AssetRow rows[1];
} Overlay5RowTable;

typedef char Overlay5AssetRowSizeCheck[sizeof(Overlay5AssetRow) == 8 ? 1 : -1];

extern void *data_ov005_02069f7c[], *data_ov005_02069fe0[];

extern Overlay5Entry *func_ov005_02069b08(u32 key);
extern void func_ov005_02069acc(Overlay5Entry *entry, u32 key);
extern void func_ov005_02066bd4(Overlay5Archive *archive, u16 group, u16 index,
                                Overlay5Asset *owner, int heap);
extern void func_ov005_02066d04(Overlay5Archive *archive, u16 index, u16 entry,
                                Overlay5BufferEntry *owner, u8 mode, int heap);
extern void func_ov005_02066dbc(Overlay5Archive *archive, u16 group, u16 index,
                                Overlay5Asset *owner, int heap);

Overlay5Asset *func_ov005_02066a7c(Overlay5Archive *archive, u16 group, u16 index,
                                   const Overlay5RowTable *table, u8 mode, int heap) {
    u32 key = index + (group << 16);
    const Overlay5AssetRow *rows;
    const Overlay5AssetRow *alternate;
    Overlay5Asset *asset = (Overlay5Asset *)func_ov005_02069b08(key);
    if (asset) return asset;
    asset = GameHeap_New(sizeof(Overlay5Asset), heap, 0, 1);
    if (asset) {
        asset->vtable = data_ov005_02069fe0;
        asset->references = 0;
        asset->key = 0;
        asset->vtable = data_ov005_02069f7c;
        asset->unknown_14 = 0;
        asset->unknown_1c = 0;
        asset->image = 0;
        asset->palette = 0;
    }
    func_ov005_02069acc((Overlay5Entry *)asset, key);
    rows = table->rows;
    alternate = &table->rows[table->count];
    if (rows[index].flags.bits.alternate) index = index + gSaveData[0x515];
    asset->row = &rows[index];
    func_ov005_02066dbc(archive, group, rows[index].image + 1, asset, heap);
    func_ov005_02066d04(archive, group, rows[index].image, (Overlay5BufferEntry *)asset, mode,
                        heap);
    func_ov005_02066bd4(archive, group, alternate[rows[index].unknown_02].image, asset, heap);
    return asset;
}
