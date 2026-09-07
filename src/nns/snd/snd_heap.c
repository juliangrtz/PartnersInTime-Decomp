#include <nns/snd.h>

extern u32 func_0203c570(void);
extern int func_0203c640(int flags);
extern void func_0203c5b0(u32 command);

NNSSndHeap *NNS_SndHeapCreate(void *start, u32 size)
{
    NNSSndHeap *heap;
    u32 aligned = (u32)start + 3;
    u32 end = (u32)start + size;
    NNSFndHeapHead *frame;
    heap = (NNSSndHeap *)(aligned & ~3);
    if ((u32)heap > end) {
        return 0;
    }
    if (end - (u32)heap < sizeof(NNSSndHeap)) {
        return 0;
    }
    frame = NNS_FndCreateFrmHeapEx(heap + 1, end - (u32)heap - sizeof(NNSSndHeap), 0);
    if (!frame) {
        return 0;
    }
    if (NNSi_SndHeapInit(heap, frame)) {
        return heap;
    }
    NNS_FndDestroyFrmHeap(frame);
    return 0;
}

void NNS_SndHeapDestroy(NNSSndHeap *heap)
{
    NNS_SndHeapClear(heap);
    NNS_FndDestroyFrmHeap(heap->frame);
}

void NNS_SndHeapClear(NNSSndHeap *heap)
{
    NNSiSndHeapSection *section;
    NNSiSndHeapBlock *block;
    int disposed = 0;
    section = NNS_FndGetPrevListObject(&heap->sections, 0);
    while (section) {
        block = NNS_FndGetPrevListObject(&section->blocks, 0);
        while (block) {
            if (block->dispose) {
                block->dispose(block + 1, block->size, block->user0, block->user1);
                disposed = 1;
            }
            block = NNS_FndGetPrevListObject(&section->blocks, block);
        }
        NNS_FndRemoveListObject(&heap->sections, section);
        section = NNS_FndGetPrevListObject(&heap->sections, 0);
    }
    NNS_FndFreeToFrmHeap(heap->frame, 3);
    if (disposed) {
        NNSi_SndHeapWaitCommands();
    }
    NNSi_SndHeapNewSection(heap);
}

void *NNS_SndHeapAlloc(NNSSndHeap *heap, u32 size, NNSSndHeapDisposeCallback dispose, u32 user0, u32 user1)
{
    NNSiSndHeapBlock *block = NNS_FndAllocFromFrmHeapEx(heap->frame, ((size + 31) & ~31) + sizeof(NNSiSndHeapBlock), 32);
    NNSiSndHeapSection *section;
    if (!block) {
        return 0;
    }
    section = NNS_FndGetPrevListObject(&heap->sections, 0);
    block->size = size;
    block->dispose = dispose;
    block->user0 = user0;
    block->user1 = user1;
    NNS_FndAppendListObject(&section->blocks, block);
    return block + 1;
}

int NNS_SndHeapSaveState(NNSSndHeap *heap)
{
    if (!NNS_FndRecordStateForFrmHeap(heap->frame, heap->sections.count)) {
        return -1;
    }
    if (NNSi_SndHeapNewSection(heap)) {
        return heap->sections.count - 1;
    }
    NNS_FndFreeByStateToFrmHeap(heap->frame, 0);
    return -1;
}

void NNS_SndHeapLoadState(NNSSndHeap *heap, int level)
{
    NNSiSndHeapSection *section;
    NNSiSndHeapBlock *block = 0;
    int disposed = 0;
    if (!level) {
        NNS_SndHeapClear(heap);
        return;
    }
    while (level < heap->sections.count) {
        section = NNS_FndGetPrevListObject(&heap->sections, 0);
        block = NNS_FndGetPrevListObject(&section->blocks, block);
        while (block) {
            if (block->dispose) {
                block->dispose(block + 1, block->size, block->user0, block->user1);
                disposed = 1;
            }
            block = NNS_FndGetPrevListObject(&section->blocks, block);
        }
        NNS_FndRemoveListObject(&heap->sections, section);
    }
    NNS_FndFreeByStateToFrmHeap(heap->frame, level);
    if (disposed) {
        NNSi_SndHeapWaitCommands();
    }
    NNS_FndRecordStateForFrmHeap(heap->frame, heap->sections.count);
    NNSi_SndHeapNewSection(heap);
}

int NNS_SndHeapGetCurrentLevel(NNSSndHeap *heap)
{
    return heap->sections.count - 1;
}

void NNSi_SndHeapInitSection(NNSiSndHeapSection *section)
{
    NNS_FndInitList(&section->blocks, 0);
}

int NNSi_SndHeapInit(NNSSndHeap *heap, NNSFndHeapHead *frame)
{
    NNS_FndInitList(&heap->sections, 12);
    heap->frame = frame;
    return NNSi_SndHeapNewSection(heap) != 0;
}

int NNSi_SndHeapNewSection(NNSSndHeap *heap)
{
    NNSiSndHeapSection *section = NNS_FndAllocFromFrmHeapEx(heap->frame, sizeof(NNSiSndHeapSection), 4);
    if (!section) {
        return 0;
    }
    NNSi_SndHeapInitSection(section);
    NNS_FndAppendListObject(&heap->sections, section);
    return 1;
}

void NNSi_SndHeapWaitCommands(void)
{
    u32 command = func_0203c570();
    func_0203c640(1);
    func_0203c5b0(command);
}
