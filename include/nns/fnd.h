#ifndef NNS_FND_H
#define NNS_FND_H

#include <nitro.h>

typedef struct NNSFndLink {
    void *previous;
    void *next;
} NNSFndLink;

typedef struct NNSFndList {
    void *head;
    void *tail;
    u16 count;
    u16 offset;
} NNSFndList;

typedef struct NNSFndHeapHead {
    u32 signature;
    NNSFndLink link;
    NNSFndList children;
    u8 *start;
    u8 *end;
    u32 attributes;
} NNSFndHeapHead;

typedef struct NNSFndFrameState {
    u32 tag;
    u8 *head;
    u8 *tail;
    struct NNSFndFrameState *previous;
} NNSFndFrameState;

typedef struct NNSFndFrameData {
    u8 *head;
    u8 *tail;
    NNSFndFrameState *state;
} NNSFndFrameData;

typedef char NNSFndHeapHeadSizeCheck[(sizeof(NNSFndHeapHead) == 36) ? 1 : -1];

void NNS_FndInitList(NNSFndList *list, u16 offset);
void NNSi_FndInitListObject(NNSFndList *list, void *object);
void NNS_FndAppendListObject(NNSFndList *list, void *object);
void NNS_FndPrependListObject(NNSFndList *list, void *object);
void NNS_FndInsertListObject(NNSFndList *list, void *next, void *object);
void NNS_FndRemoveListObject(NNSFndList *list, void *object);
void *NNS_FndGetNextListObject(NNSFndList *list, void *object);
void *NNS_FndGetPrevListObject(NNSFndList *list, void *object);

NNSFndHeapHead *NNSi_FndFindContainHeap(NNSFndList *list, const void *address);
NNSFndList *NNSi_FndFindHeapList(const void *address);
void NNSi_FndInitHeapHead(NNSFndHeapHead *heap, u32 signature, void *start, void *end, u16 options);
void NNSi_FndFinalizeHeap(NNSFndHeapHead *heap);

NNSFndHeapHead *NNSi_FndInitFrameHeap(NNSFndHeapHead *heap, void *end, u16 options);
void *NNSi_FndAllocFrameHead(NNSFndFrameData *data, u32 size, int alignment);
void *NNSi_FndAllocFrameTail(NNSFndFrameData *data, u32 size, int alignment);
void NNSi_FndFreeFrameHead(NNSFndHeapHead *heap);
void NNSi_FndFreeFrameTail(NNSFndHeapHead *heap);
NNSFndHeapHead *NNS_FndCreateFrmHeapEx(void *start, u32 size, u16 options);
void NNS_FndDestroyFrmHeap(NNSFndHeapHead *heap);
void *NNS_FndAllocFromFrmHeapEx(NNSFndHeapHead *heap, u32 size, int alignment);
void NNS_FndFreeToFrmHeap(NNSFndHeapHead *heap, int mode);
int NNS_FndRecordStateForFrmHeap(NNSFndHeapHead *heap, u32 tag);
int NNS_FndFreeByStateToFrmHeap(NNSFndHeapHead *heap, u32 tag);

#endif
