#include <nns/fnd.h>

extern int data_02060e88;
extern NNSFndList data_02060e8c;

NNSFndHeapHead *NNSi_FndFindContainHeap(NNSFndList *list, const void *address)
{
    NNSFndHeapHead *heap = NNS_FndGetNextListObject(list, 0);
    while (heap) {
        if (heap->start <= (const u8 *)address && (const u8 *)address < heap->end) {
            NNSFndHeapHead *child = NNSi_FndFindContainHeap(&heap->children, address);
            return child ? child : heap;
        }
        heap = NNS_FndGetNextListObject(list, heap);
    }
    return 0;
}

NNSFndList *NNSi_FndFindHeapList(const void *address)
{
    NNSFndList *list = &data_02060e8c;
    NNSFndHeapHead *parent = NNSi_FndFindContainHeap(list, address);
    if (parent) {
        list = &parent->children;
    }
    return list;
}

void NNSi_FndInitHeapHead(NNSFndHeapHead *heap, u32 signature, void *start, void *end, u16 options)
{
    heap->signature = signature;
    heap->start = start;
    heap->end = end;
    heap->attributes = 0;
    heap->attributes &= ~0xff;
    heap->attributes |= options & 0xff;
    NNS_FndInitList(&heap->children, 4);
    if (!data_02060e88) {
        NNS_FndInitList(&data_02060e8c, 4);
        data_02060e88 = 1;
    }
    NNS_FndAppendListObject(NNSi_FndFindHeapList(heap), heap);
}

void NNSi_FndFinalizeHeap(NNSFndHeapHead *heap)
{
    NNS_FndRemoveListObject(NNSi_FndFindHeapList(heap), heap);
}
