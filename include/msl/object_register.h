#ifndef PIT_MSL_OBJECT_REGISTER_H
#define PIT_MSL_OBJECT_REGISTER_H

/* Static objects link their cleanup records here when an overlay is loaded. */
typedef void (*MslGlobalDestructor)(void *object, int mode);
typedef struct MslDestructorNode {
    struct MslDestructorNode *next;
    MslGlobalDestructor destructor;
    void *object;
} MslDestructorNode;

#ifdef __cplusplus
extern "C" {
#endif
void *MSL_RegisterGlobalObject(void *object, MslGlobalDestructor destructor, MslDestructorNode *node);
#ifdef __cplusplus
}
#endif
#endif
