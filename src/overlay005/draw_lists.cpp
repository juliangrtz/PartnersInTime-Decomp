extern "C" {
#include <nitro.h>
#include <nitro/fx.h>
#include <game/heap.h>

extern s16 FX_SinCosTable_[];
}

/* A drawing entry: the object plus the next entry in its list. */
typedef struct Overlay5DrawNode {
    void *object;
    struct Overlay5DrawNode *next;
} Overlay5DrawNode;

/* Sixty-four ordered draw lists per screen. The head doubles as the sentinel
   that `last` points at while a list is empty. */
typedef struct Overlay5DrawList {
    Overlay5DrawNode sentinel;
    Overlay5DrawNode *last;
} Overlay5DrawList;

typedef struct Overlay5DrawLink {
    Overlay5DrawNode *entry;
    struct Overlay5DrawLink *next;
} Overlay5DrawLink;

typedef struct Overlay5DrawPool {
    Overlay5DrawNode *entries;
    Overlay5DrawLink *links;
    Overlay5DrawLink *free_list;
    Overlay5DrawLink *taken_list;
    u32 count;
} Overlay5DrawPool;

/* The drawn object: its list screen, hardware attributes and affine matrix. */
typedef struct Overlay5Object {
    u8 unknown_00[16];
    u8 screen;
    u8 unknown_11[3];
    union {
        u32 raw;
        struct { u32 reserved0 : 14, shape : 2, reserved16 : 14, size : 2; } bits;
    } attributes;
    u8 unknown_18[32];
    s16 m00, m01, m10, m11;
} Overlay5Object;

typedef char Overlay5DrawListSizeCheck[sizeof(Overlay5DrawList) == 12 ? 1 : -1];

class Overlay5DrawInterface {
public:
    virtual void update();
    virtual void draw();
    virtual void collect(void *context, u8 *first, u8 *second);
};

extern "C" {

extern Overlay5DrawPool data_ov005_0206a3f8;
extern Overlay5DrawList data_ov005_0206a40c[2][64];
/* Interleaved size/shape table; the two halves have separate symbols. */
extern const u8 data_ov005_0206a130[], data_ov005_0206a131[];

Overlay5DrawNode *func_ov005_020691ec(Overlay5DrawPool *pool);
void func_ov005_02069198(Overlay5DrawPool *pool);
void func_ov005_02069210(Overlay5DrawPool *pool);
void func_ov005_0206903c(int screen);

u32 func_ov005_02069374(void) {
    return 64;
}

void func_ov005_02069330(Overlay5Object *object, int index) {
    object->attributes.bits.size = data_ov005_0206a130[index * 2];
    object->attributes.bits.shape = data_ov005_0206a131[index * 2];
}

void func_ov005_020692c0(Overlay5Object *object, fx32 scale_x, fx32 scale_y, int angle) {
    int index = (angle & 0xffff) >> 4;
    object->m00 = (scale_x * FX_SinCosTable_[index * 2 + 1]) >> 12;
    object->m01 = -((scale_y * FX_SinCosTable_[index * 2]) >> 12);
    object->m10 = (scale_x * FX_SinCosTable_[index * 2]) >> 12;
    object->m11 = (scale_y * FX_SinCosTable_[index * 2 + 1]) >> 12;
}

void func_ov005_020692bc(void) {
}

void func_ov005_020692b8(void) {
}

Overlay5DrawPool *func_ov005_020692a0(Overlay5DrawPool *pool) {
    func_ov005_02069198(pool);
    return pool;
}

void func_ov005_02069210(Overlay5DrawPool *pool) {
    u32 index = 0;
    while (index < pool->count - 1) {
        pool->links[index].entry = &pool->entries[index];
        pool->links[index].next = &pool->links[index + 1];
        ++index;
    }
    (pool->links + pool->count - 1)->entry = &pool->entries[pool->count - 1];
    (pool->links + pool->count - 1)->next = 0;
    pool->free_list = pool->links;
    pool->taken_list = 0;
}

Overlay5DrawNode *func_ov005_020691ec(Overlay5DrawPool *pool) {
    Overlay5DrawLink *link = pool->free_list;
    Overlay5DrawNode *entry = link->entry;
    pool->free_list = link->next;
    link->next = pool->taken_list;
    pool->taken_list = link;
    return entry;
}

void func_ov005_02069198(Overlay5DrawPool *pool) {
    if (pool->entries) {
        GameHeap_DeleteArray(pool->entries);
        pool->entries = 0;
    }
    if (pool->links) {
        GameHeap_DeleteArray(pool->links);
        pool->links = 0;
    }
    pool->entries = 0;
    pool->links = 0;
    pool->free_list = 0;
    pool->taken_list = 0;
}

void func_ov005_02069124(Overlay5DrawPool *pool, u32 count) {
    if (pool->entries || pool->links) func_ov005_02069198(pool);
    pool->entries = (Overlay5DrawNode *)GameHeap_NewArray(
        count * sizeof(Overlay5DrawNode), 0, 0, 1);
    pool->links = (Overlay5DrawLink *)GameHeap_NewArray(
        count * sizeof(Overlay5DrawLink), 0, 0, 1);
    pool->count = count;
    func_ov005_02069210(pool);
}

void func_ov005_020690f0(u32 count) {
    func_ov005_02069124(&data_ov005_0206a3f8, count);
    func_ov005_0206903c(0);
    func_ov005_0206903c(1);
}

void func_ov005_020690dc(void) {
    func_ov005_02069198(&data_ov005_0206a3f8);
}

Overlay5DrawNode *func_ov005_02069084(Overlay5Object *object, int list) {
    Overlay5DrawList *entry = &data_ov005_0206a40c[object->screen][list];
    Overlay5DrawNode *node = func_ov005_020691ec(&data_ov005_0206a3f8);
    node->object = object;
    node->next = 0;
    entry->last->next = node;
    entry->last = node;
    return node;
}

void func_ov005_0206903c(int screen) {
    Overlay5DrawList *list = data_ov005_0206a40c[screen];
    int index = 0;
    do {
        list->last = &list->sentinel;
        list->last->next = 0;
        ++index;
        ++list;
    } while (index < 64);
    func_ov005_02069210(&data_ov005_0206a3f8);
}

void func_ov005_02068fe0(int screen) {
    Overlay5DrawNode *node;
    Overlay5DrawList *list = data_ov005_0206a40c[screen];
    int index = 0;
    do {
        for (node = list->sentinel.next; node && node->object; node = node->next) {
            ((Overlay5DrawInterface *)node->object)->update();
        }
        ++index;
        ++list;
    } while (index < 64);
}

void func_ov005_02068f84(int screen) {
    Overlay5DrawNode *node;
    Overlay5DrawList *list = data_ov005_0206a40c[screen];
    int index = 0;
    do {
        for (node = list->sentinel.next; node && node->object; node = node->next) {
            ((Overlay5DrawInterface *)node->object)->draw();
        }
        ++index;
        ++list;
    } while (index < 64);
}

void func_ov005_02068ee0(int screen, void *context, u8 *first, u8 *second) {
    u8 local_first = *first;
    u8 local_second = *second;
    Overlay5DrawNode *node;
    Overlay5DrawList *list = data_ov005_0206a40c[screen];
    int index = 0;
    do {
        for (node = list->sentinel.next; node && node->object; node = node->next) {
            ((Overlay5DrawInterface *)node->object)
                ->collect(context, &local_first, &local_second);
        }
        ++index;
        ++list;
    } while (index < 64);
    *first = local_first;
    *second = local_second;
}

}
