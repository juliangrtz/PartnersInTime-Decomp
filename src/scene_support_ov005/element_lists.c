/*
 * Overlay 5 element pool and update lists (overlay 5, 0x02066224-0x02066920).
 *
 * The pooled scene elements and the twelve intrusive lists they are distributed
 * over, with the per-frame update, mark and sweep passes that step them.
 *
 * The two resource mixins at the end - ResourceA and ResourceB - are the
 * attach/get/release protocol an element uses to hold a reference to a shared
 * resource; the release callback is what unbinds it when the element dies.
 */

#include <nitro.h>
#include <nitro/fs.h>
#include <game/heap.h>

typedef struct Overlay5Element Overlay5Element;
struct Overlay5Element {
    Overlay5Element *previous, *next;
    u16 flags;
    u16 list;
    void (*update)(Overlay5Element *element);
    u32 unknown_10;
    u32 unknown_14;
    void (*release)(Overlay5Element *element);
    u32 unknown_1c;
    u32 unknown_20;
    u8 unknown_24[36];
};

/* Each list keeps its head and tail markers side by side. */
typedef struct Overlay5ElementMarker {
    Overlay5Element *previous, *next;
} Overlay5ElementMarker;

typedef struct Overlay5ElementList {
    Overlay5ElementMarker markers[2];
} Overlay5ElementList;

typedef struct Overlay5ElementLink {
    Overlay5Element *element;
    struct Overlay5ElementLink *next;
} Overlay5ElementLink;

typedef struct Overlay5ElementPool {
    Overlay5Element *elements;
    Overlay5ElementLink *links;
    Overlay5ElementLink *free_list;
    Overlay5ElementLink *taken_list;
    u32 count;
} Overlay5ElementPool;

typedef char Overlay5ElementSizeCheck[sizeof(Overlay5Element) == 0x48 ? 1 : -1];

extern Overlay5ElementPool data_ov005_0206a240;
extern Overlay5ElementList data_ov005_0206a254[12];
extern u16 data_ov005_0206a230;

extern void *func_02048aac(void *memory, u32 count, u32 size, u32 header,
                           void *(*construct)(void *), void *(*destroy)(void *));
extern void func_ov005_020688cc(void *object, int copy_flag);

void func_ov005_020664a8(Overlay5Element *element);
void func_ov005_0206650c(Overlay5Element *element);
Overlay5Element *func_ov005_0206678c(Overlay5ElementPool *pool);
void func_ov005_020667b0(Overlay5ElementPool *pool, Overlay5Element *element);
void func_ov005_020667d0(Overlay5ElementPool *pool);
void func_ov005_02066738(Overlay5ElementPool *pool);
void func_ov005_020666a4(Overlay5ElementPool *pool, u32 count);

void func_ov005_0206690c(void *owner) {
    func_ov005_020688cc(((void **)owner)[20], 0);
}

void func_ov005_020668f8(void *owner) {
    func_ov005_020688cc(((void **)owner)[20], 1);
}

void *func_ov005_0206687c(const char *path, u32 *length, int heap) {
    FsFile file;
    u32 size;
    void *buffer;
    FS_InitFile(&file);
    FS_OpenFile(&file, path);
    size = file.property.file.bottom - file.property.file.top;
    buffer = GameHeap_NewArray(size, heap, 0, 1);
    size = FS_ReadFile(&file, buffer, size);
    if (length) *length = size;
    FS_CloseFile(&file);
    return buffer;
}

Overlay5ElementPool *func_ov005_02066864(Overlay5ElementPool *pool) {
    func_ov005_02066738(pool);
    return pool;
}

void func_ov005_020667d0(Overlay5ElementPool *pool) {
    u32 index = 0;
    while (index < pool->count - 1) {
        pool->links[index].element = &pool->elements[index];
        pool->links[index].next = &pool->links[index + 1];
        ++index;
    }
    (pool->links + pool->count - 1)->element = &pool->elements[pool->count - 1];
    (pool->links + pool->count - 1)->next = 0;
    pool->free_list = pool->links;
    pool->taken_list = 0;
}

void func_ov005_020667b0(Overlay5ElementPool *pool, Overlay5Element *element) {
    Overlay5ElementLink *link = pool->taken_list;
    link->element = element;
    pool->taken_list = link->next;
    link->next = pool->free_list;
    pool->free_list = link;
}

Overlay5Element *func_ov005_0206678c(Overlay5ElementPool *pool) {
    Overlay5ElementLink *link = pool->free_list;
    Overlay5Element *element = link->element;
    pool->free_list = link->next;
    link->next = pool->taken_list;
    pool->taken_list = link;
    return element;
}

void func_ov005_02066738(Overlay5ElementPool *pool) {
    if (pool->elements) {
        GameHeap_DeleteArray(pool->elements);
        pool->elements = 0;
    }
    if (pool->links) {
        GameHeap_DeleteArray(pool->links);
        pool->links = 0;
    }
    pool->elements = 0;
    pool->links = 0;
    pool->free_list = 0;
    pool->taken_list = 0;
}

void func_ov005_020666a4(Overlay5ElementPool *pool, u32 count) {
    if (pool->elements || pool->links) func_ov005_02066738(pool);
    pool->elements = func_02048aac(
        GameHeap_NewArray(count * sizeof(Overlay5Element), 0, 0, 1), count,
        sizeof(Overlay5Element), 0, 0, 0);
    pool->links = GameHeap_NewArray(count * sizeof(Overlay5ElementLink), 0, 0, 1);
    pool->count = count;
    func_ov005_020667d0(pool);
}

void func_ov005_02066638(u32 count) {
    int index;
    func_ov005_020666a4(&data_ov005_0206a240, count);
    index = 0;
    do {
        data_ov005_0206a254[index].markers[0].previous = 0;
        data_ov005_0206a254[index].markers[0].next =
            (Overlay5Element *)&data_ov005_0206a254[index].markers[1];
        data_ov005_0206a254[index].markers[1].previous =
            (Overlay5Element *)&data_ov005_0206a254[index].markers[0];
        data_ov005_0206a254[index].markers[1].next = 0;
        ++index;
    } while (index < 12);
    data_ov005_0206a230 = 0;
}

void func_ov005_02066624(void) {
    func_ov005_02066738(&data_ov005_0206a240);
}

Overlay5Element *func_ov005_0206659c(void (*update)(Overlay5Element *), int list, int marker) {
    Overlay5Element *element = func_ov005_0206678c(&data_ov005_0206a240);
    element->list = list;
    element->update = update;
    element->flags = 0;
    element->unknown_10 = 0;
    element->unknown_14 = 0;
    element->previous = data_ov005_0206a254[list].markers[marker].previous;
    element->next = (Overlay5Element *)&data_ov005_0206a254[list].markers[marker];
    element->previous->next = element;
    element->next->previous = element;
    element->release = 0;
    element->unknown_1c = 0;
    element->unknown_20 = 0;
    ++data_ov005_0206a230;
    return element;
}

void func_ov005_0206651c(int first, int last) {
    int index;
    for (index = first; index <= last; ++index) {
        Overlay5Element *element;
        for (element = data_ov005_0206a254[index].markers[0].next;
             element != (Overlay5Element *)&data_ov005_0206a254[index].markers[1];
             element = element->next) {
            if (!(element->flags & 3)) element->update(element);
        }
    }
}

void func_ov005_0206650c(Overlay5Element *element) {
    element->flags |= 2;
}

void func_ov005_020664a8(Overlay5Element *element) {
    if (element->release) {
        element->release(element);
        element->release = 0;
    }
    element->next->previous = element->previous;
    element->previous->next = element->next;
    func_ov005_020667b0(&data_ov005_0206a240, element);
    --data_ov005_0206a230;
}

void func_ov005_0206642c(int first, int last) {
    int index;
    for (index = first; index <= last; ++index) {
        Overlay5Element *element = data_ov005_0206a254[index].markers[0].next;
        while (element != (Overlay5Element *)&data_ov005_0206a254[index].markers[1]) {
            Overlay5Element *next = element->next;
            if (element->flags & 2) func_ov005_020664a8(element);
            element = next;
        }
    }
}

void func_ov005_020663d8(int list) {
    Overlay5Element *element;
    for (element = data_ov005_0206a254[list].markers[0].next;
         element != (Overlay5Element *)&data_ov005_0206a254[list].markers[1];
         element = element->next) {
        func_ov005_0206650c(element);
    }
}

void func_ov005_0206638c(int list, void (*action)(Overlay5Element *)) {
    Overlay5Element *element;
    for (element = data_ov005_0206a254[list].markers[0].next;
         element != (Overlay5Element *)&data_ov005_0206a254[list].markers[1];
         element = element->next) {
        action(element);
    }
}

void func_ov005_02066358(Overlay5Element *element, void (*update)(Overlay5Element *),
                         int invoke) {
    element->update = update;
    element->unknown_20 = 0;
    if (!invoke) return;
    update(element);
}

#include <game/overlay005_resource.h>

extern void *func_ov005_02068c80(void);
extern void func_ov005_02068c54(void *resource);
extern void *func_ov005_0206964c(void);
extern void func_ov005_020695cc(void *resource);
extern void *func_ov005_02069084(void *resource, int selector);

void Overlay5ResourceA_Release(Overlay5ResourceOwner *owner);
void Overlay5ResourceB_Release(Overlay5ResourceOwner *owner);

/* Metrowerks emits C functions in reverse source order. */
void Overlay5ResourceB_Release(Overlay5ResourceOwner *owner) {
    if (owner->resource != 0) {
        func_ov005_020695cc(owner->resource);
    }
    owner->resource = 0;
}

void *Overlay5ResourceB_Attach(Overlay5ResourceOwner *owner, void *resource,
                               int release_with_owner) {
    if (resource == 0) {
        resource = func_ov005_0206964c();
    }
    if (release_with_owner) {
        owner->release_callback = Overlay5ResourceB_Release;
    }
    owner->resource = resource;
    return resource;
}

void *Overlay5ResourceB_Get(Overlay5ResourceOwner *owner) {
    return owner->resource;
}

void Overlay5ResourceA_Release(Overlay5ResourceOwner *owner) {
    if (owner->resource != 0) {
        func_ov005_02068c54(owner->resource);
    }
    owner->resource = 0;
}

void *Overlay5ResourceA_Attach(Overlay5ResourceOwner *owner, void *resource,
                               int release_with_owner) {
    if (resource == 0) {
        resource = func_ov005_02068c80();
    }
    if (release_with_owner) {
        owner->release_callback = Overlay5ResourceA_Release;
    }
    owner->resource = resource;
    return resource;
}

void *Overlay5ResourceA_Get(Overlay5ResourceOwner *owner) {
    return owner->resource;
}

void Overlay5ResourceA_ReleaseCallback(Overlay5ResourceOwner *owner) {
    Overlay5ResourceA_Release(owner);
}

void *Overlay5ResourceB_ApplySelector(Overlay5ResourceOwner *owner) {
    void *resource = Overlay5ResourceB_Get(owner);
    return func_ov005_02069084(resource, owner->selector & 0xFF);
}

void *Overlay5ResourceA_ApplySelector(Overlay5ResourceOwner *owner) {
    void *resource = Overlay5ResourceA_Get(owner);
    return func_ov005_02069084(resource, owner->selector & 0xFF);
}
