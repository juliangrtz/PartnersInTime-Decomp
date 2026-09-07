#include <nitro.h>
#include <game/heap.h>

/* An overlay-5 resource holder: two owned buffers and two registry entries. */
typedef struct Overlay5Holder {
    void **vtable;
    u8 unknown_04[16];
    void *buffer;
    u8 unknown_18[4];
    void *table;
    void *entry;
    void *shared;
} Overlay5Holder;

extern void *data_ov005_02069f7c[], *data_ov005_02069fb0[], *data_ov005_02069fe0[];

extern void func_ov005_02069a54(void *entry);
extern void GXi_SetMasterBrightness(vu16 *address, int brightness);

void func_ov005_0206781c(int screen, int brightness) {
    if (screen != 1) GXi_SetMasterBrightness((vu16 *)0x0400006c, brightness);
    if (screen == 0) return;
    GXi_SetMasterBrightness((vu16 *)0x0400106c, brightness);
}

void func_ov005_020677d0(Overlay5Holder *holder) {
    if (holder->buffer) {
        GameHeap_DeleteArray(holder->buffer);
        holder->buffer = 0;
    }
    if (holder->table) {
        GameHeap_DeleteArray(holder->table);
        holder->table = 0;
    }
    holder->entry = 0;
    holder->shared = 0;
}

void func_ov005_0206775c(Overlay5Holder *holder) {
    if (holder->buffer) {
        GameHeap_DeleteArray(holder->buffer);
        holder->buffer = 0;
    }
    if (holder->table) {
        GameHeap_DeleteArray(holder->table);
        holder->table = 0;
    }
    if (holder->entry) {
        func_ov005_02069a54(holder->entry);
        holder->entry = 0;
    }
    if (holder->shared) {
        func_ov005_02069a54(holder->shared);
        holder->shared = 0;
    }
}

void func_ov005_02067730(Overlay5Holder *holder) {
    if (!holder->buffer) return;
    GameHeap_DeleteArray(holder->buffer);
    holder->buffer = 0;
}

void func_ov005_02067704(Overlay5Holder *holder) {
    if (!holder->buffer) return;
    GameHeap_DeleteArray(holder->buffer);
    holder->buffer = 0;
}

Overlay5Holder *func_ov005_020676e0(Overlay5Holder *holder) {
    holder->vtable = data_ov005_02069fe0;
    GameHeap_Delete(holder);
    return holder;
}

void func_ov005_020676d0(Overlay5Holder *holder) {
    holder->vtable = data_ov005_02069fe0;
}

Overlay5Holder *func_ov005_020676a0(Overlay5Holder *holder) {
    holder->vtable = data_ov005_02069f7c;
    holder->vtable = data_ov005_02069fe0;
    GameHeap_Delete(holder);
    return holder;
}

void func_ov005_02067684(Overlay5Holder *holder) {
    holder->vtable = data_ov005_02069f7c;
    holder->vtable = data_ov005_02069fe0;
}

Overlay5Holder *func_ov005_02067654(Overlay5Holder *holder) {
    holder->vtable = data_ov005_02069fb0;
    holder->vtable = data_ov005_02069fe0;
    GameHeap_Delete(holder);
    return holder;
}

void func_ov005_02067638(Overlay5Holder *holder) {
    holder->vtable = data_ov005_02069fb0;
    holder->vtable = data_ov005_02069fe0;
}
