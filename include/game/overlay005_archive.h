#ifndef PIT_GAME_OVERLAY005_ARCHIVE_H
#define PIT_GAME_OVERLAY005_ARCHIVE_H

#include <game/archive_io.h>

/* One queued archive read: list links, the free-list link, the shared request
   record, the archive it reads from and the completion callback. */
typedef struct Overlay5ArchiveRequest {
    struct Overlay5ArchiveRequest *previous, *next, *free_next;
    ArchiveCompressedRequest request;
    u8 kind;
    u8 archive;
    u16 entry;
    void *owner;
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

/* A reference-counted resource entry kept on the overlay's registry list. */
typedef struct Overlay5Entry {
    const void *vtable;
    struct Overlay5Entry *previous, *next;
    int references;
    u32 key;
    u32 unknown_14;
} Overlay5Entry;

/* The read variant replaces the trailing word with the decoded buffer and adds
   its length. */
typedef struct Overlay5BufferEntry {
    const void *vtable;
    Overlay5Entry *previous, *next;
    int references;
    u32 key;
    void *buffer;
    u32 length;
} Overlay5BufferEntry;

/* An asset holds the two entries a graphic needs plus the row it came from. */
typedef struct Overlay5Asset {
    const void *vtable;
    Overlay5Entry *previous, *next;
    int references;
    u32 key;
    u32 unknown_14;
    u32 unknown_18;
    u32 unknown_1c;
    Overlay5BufferEntry *image;
    Overlay5BufferEntry *palette;
    const void *row;
} Overlay5Asset;

typedef char Overlay5BufferEntrySizeCheck[sizeof(Overlay5BufferEntry) == 0x1c ? 1 : -1];
typedef char Overlay5AssetSizeCheck[sizeof(Overlay5Asset) == 0x2c ? 1 : -1];

#endif
