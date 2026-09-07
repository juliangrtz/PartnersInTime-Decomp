#include <game/archive_io.h>
#include <game/battle_archive.h>
#include <game/heap.h>

/* One queued archive read: list links, the free-list link, the shared request
   record and the completion callback. */
typedef struct Overlay5ArchiveRequest {
    struct Overlay5ArchiveRequest *previous, *next, *free_next;
    ArchiveCompressedRequest request;
    u8 kind;
    u8 reserved4d[7];
    void (*callback)(struct Overlay5ArchiveRequest *request);
    u32 key;
} Overlay5ArchiveRequest;

/* One opened archive: its descriptor and the offset table read for it. */
typedef struct Overlay5ArchiveTable {
    const void *descriptor;
    u32 *offsets;
} Overlay5ArchiveTable;

typedef struct Overlay5Archive {
    ArchiveCompressedIO base;
    u8 reserved340[0x100];
    Overlay5ArchiveTable tables[12];
    Overlay5ArchiveRequest requests[128];
    Overlay5ArchiveRequest head;
    Overlay5ArchiveRequest tail;
    Overlay5ArchiveRequest *free_list;
} Overlay5Archive;

typedef char Overlay5ArchiveRequestSizeCheck[sizeof(Overlay5ArchiveRequest) == 0x5c ? 1 : -1];
typedef char Overlay5ArchiveSizeCheck[sizeof(Overlay5Archive) == 0x335c ? 1 : -1];

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
