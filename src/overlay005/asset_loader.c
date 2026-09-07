#include <game/overlay005_archive.h>
#include <game/battle_archive.h>
#include <game/heap.h>

/* The read request this file builds on the stack is four bytes longer than the
   plain archive read record. */
typedef struct Overlay5ReadRequest {
    ArchiveReadRequest read;
    u32 reserved28;
} Overlay5ReadRequest;

extern void *data_ov005_02069fb0[], *data_ov005_02069fe0[];

extern void MI_CpuFill8(void *destination, u8 value, u32 size);
extern void MI_UncompressLZ16(const void *source, void *destination);
extern void MI_UncompressHuffman(const void *source, void *destination);
extern void MI_UncompressRL16(const void *source, void *destination);

extern Overlay5Entry *func_ov005_02069b08(u32 key);
extern void func_ov005_02069acc(Overlay5Entry *entry, u32 key);
extern Overlay5ArchiveRequest *func_ov005_02067184(Overlay5Archive *archive, u32 key);
extern Overlay5ArchiveRequest *func_ov005_02067174(Overlay5Archive *archive, u32 group,
                                                  u32 entry);
extern void func_ov005_0206735c(Overlay5Archive *archive, Overlay5ArchiveRequest *request);
extern void func_ov005_020668f8(void *owner);
extern void func_ov005_0206690c(void *owner);

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
