#include <nitro.h>

typedef struct MslDestructorNode {
    struct MslDestructorNode *next;
    void (*destructor)(void *object, int mode);
    void *object;
} MslDestructorNode;

typedef struct MslExceptionEntry {
    u32 start;
    u32 length_and_flags;
    u32 information;
} MslExceptionEntry;

extern MslDestructorNode *__global_destructor_chain;
extern void (*data_0204ff6c[])(void);

void MSL_RunGlobalConstructors(void)
{
    void (**constructor)(void) = data_0204ff6c;
    for (; constructor && *constructor; ++constructor) {
        (*constructor)();
    }
}

void MSL_DestroyGlobalChain(void)
{
    MslDestructorNode *node;
    while ((node = __global_destructor_chain) != 0) {
        __global_destructor_chain = node->next;
        node->destructor(node->object, -1);
    }
}

const MslExceptionEntry *MSL_FindExceptionEntry(const MslExceptionEntry *table, int count, u32 address)
{
    int first;
    int last = count - 1;
    first = 0;
    while (first <= last) {
        int middle = (first + last) >> 1;
        u32 start = table[middle].start;
        if (address < start) {
            last = middle - 1;
        } else {
            if (address > start + (table[middle].length_and_flags & ~1)) {
                first = middle + 1;
            } else {
                return &table[middle];
            }
        }
    }
    return 0;
}
