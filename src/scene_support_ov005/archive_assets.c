/*
 * Overlay 5 archive and asset loading (overlay 5, 0x02066A7C-0x02067638).
 *
 * The shared archive service the scenes use: twelve archive slots, each with its
 * descriptor and offset table, and a 128-entry request queue pumped once per
 * frame. Entries can be read synchronously or asynchronously, and the LZ,
 * Huffman and run-length variants are decompressed on the way out.
 *
 * Most functions here are still address-named; what is established is the slot
 * and request structure, not yet the meaning of each caller.
 */

#include <game/overlay005_archive.h>
#include <game/battle_archive.h>
#include <game/heap.h>

extern void *data_ov005_02069f68;

extern void MI_CpuFill8(void *destination, u8 value, u32 size);

void func_ov005_02067424(Overlay5Archive *archive, u8 index);
void func_ov005_020672ac(Overlay5Archive *archive, Overlay5ArchiveRequest *request);
int func_ov005_02067318(Overlay5Archive *archive, Overlay5ArchiveRequest *request);
Overlay5ArchiveRequest *func_ov005_02067184(Overlay5Archive *archive, u32 key);
int func_ov005_02067248(Overlay5Archive *archive, u32 key);

Overlay5Archive *func_ov005_02067584(Overlay5Archive *archive, u32 priority, u32 unused,
                                     void *argument, int dma) {
    int index;
    ArchiveCompressedIO_InitComplete(&archive->base, priority, unused, argument, dma);
    archive->base.base.vtable = &data_ov005_02069f68;
    MI_CpuFill8(archive->tables, 0, sizeof(archive->tables));
    MI_CpuFill8(archive->requests, 0, sizeof(archive->requests));
    for (index = 0; index < 127; ++index) {
        archive->requests[index].free_next = &archive->requests[index + 1];
    }
    archive->free_list = archive->requests;
    archive->requests[127].free_next = 0;
    archive->head.previous = 0;
    archive->head.next = &archive->tail;
    archive->tail.previous = &archive->head;
    archive->tail.next = 0;
    return archive;
}

Overlay5Archive *func_ov005_02067538(Overlay5Archive *archive) {
    int index = 0;
    archive->base.base.vtable = &data_ov005_02069f68;
    do {
        func_ov005_02067424(archive, index);
        ++index;
    } while (index < 12);
    ArchiveCompressedIO_DestroyBase(&archive->base);
    return archive;
}

Overlay5Archive *func_ov005_020674e4(Overlay5Archive *archive) {
    int index = 0;
    archive->base.base.vtable = &data_ov005_02069f68;
    do {
        func_ov005_02067424(archive, index);
        ++index;
    } while (index < 12);
    ArchiveCompressedIO_DestroyBase(&archive->base);
    GameHeap_Delete(archive);
    return archive;
}

void func_ov005_02067468(Overlay5Archive *archive, u8 index, const void *descriptor, int heap) {
    u32 size;
    u32 *offsets;
    func_ov005_02067424(archive, index);
    size = BattleArchive_GetDescriptorSize(archive, descriptor);
    offsets = (u32 *)GameHeap_NewArray(size * sizeof(u32), heap, 0, 1);
    BattleArchive_Open(archive, (const u8 *)offsets, size, descriptor);
    ArchiveIO_WaitOpen(&archive->base.base, descriptor);
    archive->tables[index].descriptor = descriptor;
    archive->tables[index].offsets = offsets;
}

void func_ov005_02067424(Overlay5Archive *archive, u8 index) {
    if (!archive->tables[index].offsets) return;
    GameHeap_DeleteArray(archive->tables[index].offsets);
    archive->tables[index].offsets = 0;
}

void func_ov005_020673c4(Overlay5Archive *archive) {
    Overlay5ArchiveRequest *request;
    ArchiveIO_UpdateCompressed(&archive->base.base);
    for (request = archive->head.next; request != &archive->tail; request = request->next) {
        if (func_ov005_02067318(archive, request)) func_ov005_020672ac(archive, request);
    }
}

void func_ov005_0206735c(Overlay5Archive *archive, Overlay5ArchiveRequest *request) {
    switch (request->kind) {
    case 0: {
        ArchiveCompressedRequest *io = &request->request;
        BattleArchive_ReadAsync(archive, io, 0, (const u8 *)io->read.offsets, io->read.path, 0);
        break;
    }
    case 1: {
        ArchiveCompressedRequest *io = &request->request;
        ArchiveIO_QueueCompressedRead(&archive->base.base, io, io->destination);
        break;
    }
    }
}

int func_ov005_02067318(Overlay5Archive *archive, Overlay5ArchiveRequest *request) {
    switch (request->kind) {
    case 0:
        if (!(request->request.read.state & 3)) return 1;
        break;
    case 1:
        if (!request->request.compressed_state) return 1;
        break;
    }
    return 0;
}

void func_ov005_020672ac(Overlay5Archive *archive, Overlay5ArchiveRequest *request) {
    Overlay5ArchiveRequest *previous;
    Overlay5ArchiveRequest *next;
    if (request->callback) request->callback(request);
    request->next->previous = request->previous;
    previous = request->next->previous;
    request->previous->next = request->next;
    next = request->previous->next;
    request->free_next = archive->free_list;
    archive->free_list = request;
    request->previous = previous;
    request->next = next;
}

int func_ov005_02067288(Overlay5Archive *archive) {
    return archive->head.next == &archive->tail;
}

int func_ov005_02067248(Overlay5Archive *archive, u32 key) {
    Overlay5ArchiveRequest *request;
    for (request = archive->head.next; request != &archive->tail; request = request->next) {
        if (request->key == key) return 0;
    }
    return 1;
}

int func_ov005_02067238(Overlay5Archive *archive, u32 group, u32 entry) {
    return func_ov005_02067248(archive, entry + (group << 16));
}

void func_ov005_020671e4(Overlay5Archive *archive) {
    Overlay5ArchiveRequest *request;
    for (request = archive->head.next; request != &archive->tail; request = request->next) {
        ArchiveIO_WaitCompressedRead(&archive->base.base, &request->request.read);
        func_ov005_020672ac(archive, request);
    }
}

Overlay5ArchiveRequest *func_ov005_02067184(Overlay5Archive *archive, u32 key) {
    Overlay5ArchiveRequest *request = archive->free_list;
    archive->free_list = request->free_next;
    request->previous = archive->tail.previous;
    request->next = &archive->tail;
    request->previous->next = request;
    request->next->previous = request;
    request->key = key;
    request->callback = 0;
    MI_CpuFill8(&request->request, 0, sizeof(request->request));
    return request;
}

Overlay5ArchiveRequest *func_ov005_02067174(Overlay5Archive *archive, u32 group, u32 entry) {
    return func_ov005_02067184(archive, entry + (group << 16));
}

/* The read request this file builds on the stack is four bytes longer than the
   plain archive read record. */
typedef struct Overlay5ReadRequest {
    ArchiveReadRequest read;
    u32 reserved28;
} Overlay5ReadRequest;

extern void *data_ov005_02069fb0[], *data_ov005_02069fe0[];

extern void MI_UncompressLZ16(const void *source, void *destination);
extern void MI_UncompressHuffman(const void *source, void *destination);
extern void MI_UncompressRL16(const void *source, void *destination);

extern Overlay5Entry *func_ov005_02069b08(u32 key);
extern void func_ov005_02069acc(Overlay5Entry *entry, u32 key);
extern Overlay5ArchiveRequest *func_ov005_02067174(Overlay5Archive *archive, u32 group,
                                                  u32 entry);
extern void func_ov005_0206735c(Overlay5Archive *archive, Overlay5ArchiveRequest *request);
extern void func_ov005_020668f8(void *owner);
extern void func_ov005_0206690c(void *owner);

void *Overlay5Archive_ReadEntry(Overlay5Archive *archive, int index, u16 entry, u32 *length,
                          int heap) {
    Overlay5ReadRequest request;
    MI_CpuFill8(&request, 0, sizeof(request));
    request.read.path = archive->tables[index].descriptor;
    request.read.offsets = archive->tables[index].offsets;
    request.read.entry = entry;
    request.read.limit =
        BattleArchive_GetEntrySize(archive, (const u8 *)request.read.offsets, request.read.entry);
    if (!request.read.limit) return 0;
    request.read.destination = GameHeap_NewArray(request.read.limit, heap, 0, 1);
    BattleArchive_ReadAsync(archive, &request, 0, (const u8 *)request.read.offsets,
                            request.read.path, 0);
    ArchiveIO_WaitCompressedRead(&archive->base.base, (ArchiveReadRequest *)&request);
    if (length) *length = request.read.limit;
    return request.read.destination;
}

void *func_ov005_02066f78(Overlay5Archive *archive, int index, u16 entry, u32 *length,
                          int heap) {
    Overlay5ReadRequest request;
    void *compressed;
    u32 size;
    void *output;
    int alternate;
    MI_CpuFill8(&request, 0, sizeof(request));
    request.read.path = archive->tables[index].descriptor;
    request.read.offsets = archive->tables[index].offsets;
    request.read.entry = entry;
    request.read.limit =
        BattleArchive_GetEntrySize(archive, (const u8 *)request.read.offsets, request.read.entry);
    if (!request.read.limit) return 0;
    alternate = 1;
    if (heap == 1) alternate = 0;
    compressed = GameHeap_NewArray(request.read.limit, alternate, 0, 1);
    request.read.destination = compressed;
    BattleArchive_ReadAsync(archive, &request, 0, (const u8 *)request.read.offsets,
                            request.read.path, 0);
    ArchiveIO_WaitCompressedRead(&archive->base.base, (ArchiveReadRequest *)&request);
    size = *(u32 *)request.read.destination >> 8;
    output = GameHeap_NewArray(size, heap, 0, 1);
    switch (*(u32 *)request.read.destination & 0xf0) {
    case 0x10: MI_UncompressLZ16(request.read.destination, output); break;
    case 0x20: MI_UncompressHuffman(request.read.destination, output); break;
    case 0x30: MI_UncompressRL16(request.read.destination, output); break;
    }
    GameHeap_DeleteArray(request.read.destination);
    if (length) *length = size;
    return output;
}

u32 func_ov005_02066ed4(Overlay5Archive *archive, u8 index, u16 entry, void **output,
                        int heap) {
    Overlay5ArchiveRequest *request = func_ov005_02067174(archive, index, entry);
    ArchiveCompressedRequest *io;
    request->archive = index;
    request->kind = 0;
    request->entry = entry;
    io = &request->request;
    io->read.path = archive->tables[request->archive].descriptor;
    io->read.offsets = archive->tables[request->archive].offsets;
    io->read.entry = request->entry;
    io->read.limit =
        BattleArchive_GetEntrySize(archive, (const u8 *)io->read.offsets, io->read.entry);
    io->read.destination = GameHeap_NewArray(io->read.limit, heap, 0, 1);
    *output = io->read.destination;
    func_ov005_0206735c(archive, request);
    return io->read.limit;
}

void func_ov005_02066dbc(Overlay5Archive *archive, u16 group, u16 index,
                         Overlay5Asset *owner, int heap) {
    u32 key = index + (group << 16) + 0x1000000;
    Overlay5ArchiveRequest *request;
    ArchiveCompressedRequest *io;
    Overlay5BufferEntry *entry = (Overlay5BufferEntry *)func_ov005_02069b08(key);
    if (entry) {
        owner->image = entry;
        return;
    }
    entry = GameHeap_New(sizeof(Overlay5BufferEntry), heap, 0, 1);
    if (entry) {
        entry->vtable = data_ov005_02069fe0;
        entry->references = 0;
        entry->key = 0;
        entry->vtable = data_ov005_02069fb0;
        entry->buffer = 0;
    }
    func_ov005_02069acc((Overlay5Entry *)entry, key);
    request = func_ov005_02067184(archive, owner->key);
    request->archive = group;
    request->kind = 1;
    request->entry = index;
    io = &request->request;
    io->read.entry = request->entry;
    ArchiveIO_PrepareCompressedRead(&archive->base.base, io,
                                    archive->tables[request->archive].offsets,
                                    archive->tables[request->archive].descriptor);
    io->destination = GameHeap_NewArray(io->output_size, heap, 0, 1);
    func_ov005_0206735c(archive, request);
    {
        u32 length = io->output_size;
        void *buffer = io->destination;
        entry->buffer = buffer;
        entry->length = length;
    }
    owner->image = entry;
}

void func_ov005_02066d04(Overlay5Archive *archive, u16 index, u16 entry,
                         Overlay5BufferEntry *owner, u8 mode, int heap) {
    Overlay5ArchiveRequest *request = func_ov005_02067184(archive, owner->key);
    ArchiveCompressedRequest *io;
    request->archive = index;
    request->kind = 1;
    request->entry = entry;
    io = &request->request;
    if (mode == 0) request->callback = (void (*)(Overlay5ArchiveRequest *))func_ov005_0206690c;
    else request->callback = (void (*)(Overlay5ArchiveRequest *))func_ov005_020668f8;
    io->read.entry = request->entry;
    ArchiveIO_PrepareCompressedRead(&archive->base.base, io,
                                    archive->tables[request->archive].offsets,
                                    archive->tables[request->archive].descriptor);
    io->destination = GameHeap_NewArray(io->output_size, heap, 0, 1);
    func_ov005_0206735c(archive, request);
    request->owner = owner;
    {
        u32 length = io->output_size;
        void *buffer = io->destination;
        owner->buffer = buffer;
        owner->length = length;
    }
}

void func_ov005_02066bd4(Overlay5Archive *archive, u16 group, u16 index,
                         Overlay5Asset *owner, int heap) {
    u32 key = index + (group << 16) + 0x2000000;
    Overlay5ArchiveRequest *request;
    ArchiveCompressedRequest *io;
    Overlay5BufferEntry *entry = (Overlay5BufferEntry *)func_ov005_02069b08(key);
    if (entry) {
        owner->palette = entry;
        return;
    }
    entry = GameHeap_New(sizeof(Overlay5BufferEntry), heap, 0, 1);
    if (entry) {
        entry->vtable = data_ov005_02069fe0;
        entry->references = 0;
        entry->key = 0;
        entry->vtable = data_ov005_02069fb0;
        entry->buffer = 0;
    }
    func_ov005_02069acc((Overlay5Entry *)entry, key);
    request = func_ov005_02067184(archive, owner->key);
    request->archive = group;
    request->kind = 0;
    request->entry = index;
    io = &request->request;
    io->read.path = archive->tables[request->archive].descriptor;
    io->read.offsets = archive->tables[request->archive].offsets;
    io->read.entry = request->entry;
    io->read.limit =
        BattleArchive_GetEntrySize(archive, (const u8 *)io->read.offsets, io->read.entry);
    io->read.destination = GameHeap_NewArray(io->read.limit, heap, 0, 1);
    func_ov005_0206735c(archive, request);
    {
        u32 length = io->read.limit;
        void *buffer = io->read.destination;
        entry->buffer = buffer;
        entry->length = length;
    }
    owner->palette = entry;
}

#include <game/save_data.h>

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
