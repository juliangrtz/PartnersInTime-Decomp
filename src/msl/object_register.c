#include <nitro.h>

typedef struct MslDestructorNode {
    struct MslDestructorNode *next;
    void (*destructor)(void *object, int mode);
    void *object;
} MslDestructorNode;

extern MslDestructorNode *__global_destructor_chain;
extern void (*data_02059e98)(void);

void MSL_Terminate(void)
{
    data_02059e98();
}

void *MSL_RegisterGlobalObject(void *object, void (*destructor)(void *, int), MslDestructorNode *node)
{
    node->next = __global_destructor_chain;
    node->destructor = destructor;
    node->object = object;
    __global_destructor_chain = node;
    return object;
}
