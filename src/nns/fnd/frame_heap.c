#include <nns/fnd.h>

extern void func_0203b7a0(u32 value, void *destination, u32 size);

static inline NNSFndFrameData *NNSi_FndFrameData(NNSFndHeapHead *heap)
{
    return (NNSFndFrameData *)(heap + 1);
}

static inline u16 NNSi_FndHeapOptions(NNSFndFrameData *data)
{
    return ((NNSFndHeapHead *)data - 1)->attributes & 0xff;
}

static inline void NNSi_FndClearAllocation(NNSFndFrameData *data, void *start, u32 size)
{
    if (NNSi_FndHeapOptions(data) & 1) {
        volatile u32 fill = 0;
        func_0203b7a0(fill, start, size);
    }
}

NNSFndHeapHead *NNSi_FndInitFrameHeap(NNSFndHeapHead *heap, void *end, u16 options)
{
    NNSFndFrameData *data = NNSi_FndFrameData(heap);
    NNSi_FndInitHeapHead(heap, 0x46524d48, data + 1, end, options);
    data->head = heap->start;
    data->tail = heap->end;
    data->state = 0;
    return heap;
}

void *NNSi_FndAllocFrameHead(NNSFndFrameData *data, u32 size, int alignment)
{
    u8 *start = (u8 *)(~(alignment - 1) & ((alignment - 1) + (u32)data->head));
    u8 *end = (u8 *)(size + (u32)start);
    if (end > data->tail) {
        return 0;
    }
    NNSi_FndClearAllocation(data, data->head, end - data->head);
    data->head = end;
    return start;
}

void *NNSi_FndAllocFrameTail(NNSFndFrameData *data, u32 size, int alignment)
{
    u8 *end = data->tail;
    u8 *start = (u8 *)(~(alignment - 1) & ((u32)end - size));
    if (start < data->head) {
        return 0;
    }
    {
        u32 length = end - start;
        NNSi_FndClearAllocation(data, start, length);
    }
    data->tail = start;
    return start;
}

void NNSi_FndFreeFrameHead(NNSFndHeapHead *heap)
{
    NNSFndFrameData *data = NNSi_FndFrameData(heap);
    data->head = heap->start;
    data->state = 0;
}

void NNSi_FndFreeFrameTail(NNSFndHeapHead *heap)
{
    NNSFndFrameData *data = NNSi_FndFrameData(heap);
    NNSFndFrameState *state = data->state;
    for (; state; state = state->previous) {
        state->tail = heap->end;
    }
    data->tail = heap->end;
}

NNSFndHeapHead *NNS_FndCreateFrmHeapEx(void *start, u32 size, u16 options)
{
    u32 end = (size + (u32)start) & ~3;
    u32 begin = ((u32)start + 3) & ~3;
    if (begin > end || end - begin < 48) {
        return 0;
    }
    return NNSi_FndInitFrameHeap((NNSFndHeapHead *)begin, (void *)end, options);
}

void NNS_FndDestroyFrmHeap(NNSFndHeapHead *heap)
{
    NNSi_FndFinalizeHeap(heap);
}

void *NNS_FndAllocFromFrmHeapEx(NNSFndHeapHead *heap, u32 size, int alignment)
{
    NNSFndFrameData *data = NNSi_FndFrameData(heap);
    if (!size) {
        size = 1;
    }
    size = (size + 3) & ~3;
    if (alignment >= 0) {
        return NNSi_FndAllocFrameHead(data, size, alignment);
    } else {
        return NNSi_FndAllocFrameTail(data, size, -alignment);
    }
}

void NNS_FndFreeToFrmHeap(NNSFndHeapHead *heap, int mode)
{
    if (mode & 1) {
        NNSi_FndFreeFrameHead(heap);
    }
    if (mode & 2) {
        NNSi_FndFreeFrameTail(heap);
    }
}

int NNS_FndRecordStateForFrmHeap(NNSFndHeapHead *heap, u32 tag)
{
    NNSFndFrameData *data = NNSi_FndFrameData(heap);
    u8 *head = data->head;
    NNSFndFrameState *state = NNSi_FndAllocFrameHead(data, sizeof(NNSFndFrameState), 4);
    if (!state) {
        return 0;
    }
    state->tag = tag;
    state->head = head;
    state->tail = data->tail;
    state->previous = data->state;
    data->state = state;
    return 1;
}

int NNS_FndFreeByStateToFrmHeap(NNSFndHeapHead *heap, u32 tag)
{
    NNSFndFrameData *data = NNSi_FndFrameData(heap);
    NNSFndFrameState *state = data->state;
    if (tag) {
        while (state) {
            if (state->tag == tag) {
                break;
            }
            state = state->previous;
        }
    }
    if (!state) {
        return 0;
    }
    data->head = state->head;
    data->tail = state->tail;
    data->state = state->previous;
    return 1;
}
