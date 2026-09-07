#include <nns/fnd.h>

static inline NNSFndLink *NNSi_FndGetLink(NNSFndList *list, void *object)
{
    return (NNSFndLink *)((u8 *)object + list->offset);
}

void NNS_FndInitList(NNSFndList *list, u16 offset)
{
    list->head = 0;
    list->tail = 0;
    list->count = 0;
    list->offset = offset;
}

void NNSi_FndInitListObject(NNSFndList *list, void *object)
{
    NNSFndLink *link = NNSi_FndGetLink(list, object);
    link->next = 0;
    link->previous = 0;
    list->head = object;
    list->tail = object;
    ++list->count;
}

void NNS_FndAppendListObject(NNSFndList *list, void *object)
{
    NNSFndLink *link;
    if (!list->head) {
        NNSi_FndInitListObject(list, object);
        return;
    }
    link = NNSi_FndGetLink(list, object);
    link->previous = list->tail;
    link->next = 0;
    NNSi_FndGetLink(list, list->tail)->next = object;
    list->tail = object;
    ++list->count;
}

void NNS_FndPrependListObject(NNSFndList *list, void *object)
{
    NNSFndLink *link;
    if (!list->head) {
        NNSi_FndInitListObject(list, object);
        return;
    }
    link = NNSi_FndGetLink(list, object);
    link->previous = 0;
    link->next = list->head;
    NNSi_FndGetLink(list, list->head)->previous = object;
    list->head = object;
    ++list->count;
}

void NNS_FndInsertListObject(NNSFndList *list, void *next, void *object)
{
    NNSFndLink *next_link;
    NNSFndLink *link;
    void *previous;
    NNSFndLink *previous_link;
    if (!next) {
        NNS_FndAppendListObject(list, object);
        return;
    }
    if (next == list->head) {
        NNS_FndPrependListObject(list, object);
        return;
    }
    link = NNSi_FndGetLink(list, object);
    next_link = NNSi_FndGetLink(list, next);
    previous = next_link->previous;
    previous_link = NNSi_FndGetLink(list, previous);
    link->previous = previous;
    link->next = next;
    previous_link->next = object;
    NNSi_FndGetLink(list, next)->previous = object;
    ++list->count;
}

void NNS_FndRemoveListObject(NNSFndList *list, void *object)
{
    NNSFndLink *link = NNSi_FndGetLink(list, object);
    if (!link->previous) {
        list->head = link->next;
    } else {
        NNSi_FndGetLink(list, link->previous)->next = link->next;
    }
    if (!link->next) {
        list->tail = link->previous;
    } else {
        NNSi_FndGetLink(list, link->next)->previous = link->previous;
    }
    link->previous = 0;
    link->next = 0;
    --list->count;
}

void *NNS_FndGetNextListObject(NNSFndList *list, void *object)
{
    return !object ? list->head : NNSi_FndGetLink(list, object)->next;
}

void *NNS_FndGetPrevListObject(NNSFndList *list, void *object)
{
    return !object ? list->tail : NNSi_FndGetLink(list, object)->previous;
}
