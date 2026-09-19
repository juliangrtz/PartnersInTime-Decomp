extern "C" {
#include <nitro.h>
}

/* A reference-counted resource entry kept on one intrusive list. */
typedef struct Overlay5Entry {
    const void *vtable;
    struct Overlay5Entry *previous, *next;
    int references;
    u32 key;
    u32 unknown_14;
} Overlay5Entry;

/* The list embeds its own head and tail markers. */
typedef struct Overlay5EntryList {
    Overlay5Entry head;
    u32 reserved18;
    Overlay5Entry tail;
} Overlay5EntryList;

class Overlay5EntryInterface {
public:
    virtual void reserved00();
    virtual void destroy();
    virtual void release();
    virtual void finish();
};

/* One queued sound request; `state` carries the pending flag and the sound id. */
typedef struct Overlay5SoundSlot {
    u16 state;
    u16 priority;
    u16 delay;
    u16 wait;
} Overlay5SoundSlot;

extern "C" {

extern Overlay5EntryList data_ov005_0206aa40;
extern Overlay5Entry data_ov005_0206aa5c;
extern Overlay5SoundSlot data_ov005_0206aa78[32];
extern void *data_ov005_02069fb0[], *data_ov005_02069fe0[];

extern void MI_CpuFill8(void *destination, u8 value, u32 size);
extern void GameAudio_PlayEncoded(s16 sound);
extern void func_020488bc(void *object, int kind, u32 size, void (*destroy)(void *));
extern void func_ov005_02067638(void *object);

void func_ov005_02069b54(void);

void func_ov005_02069d50(void) {
    MI_CpuFill8(data_ov005_0206aa78, 0, sizeof(data_ov005_0206aa78));
}

void func_ov005_02069cb8(void) {
    Overlay5SoundSlot *slot = data_ov005_0206aa78;
    int index = 0;
    do {
        u16 state = slot->state;
        if (state) {
            if (state & 0x8000) {
                if (slot->delay) {
                    slot->delay = slot->delay - 1;
                } else {
                    slot->state = state & 0xfff;
                    GameAudio_PlayEncoded(slot->state - 1);
                }
            } else if (slot->wait) {
                slot->wait = slot->wait - 1;
            } else {
                slot->state = 0;
            }
        }
        ++index;
        ++slot;
    } while (index < 32);
}

void func_ov005_02069bcc(int sound, u16 delay, u16 wait, u16 priority) {
    Overlay5SoundSlot *slot;
    u16 request = sound + 1;
    u16 chosen = 0;
    u16 fallback = 0;
    u16 lowest = 0xffff;
    int index = 0;
    slot = data_ov005_0206aa78;
    do {
        u16 state = slot->state;
        if ((state & 0xfff) == request) return;
        if (state == 0) {
            chosen = index + 1;
            break;
        }
        if (slot->priority < priority || slot->priority == 0) {
            if (slot->priority < lowest) {
                fallback = index + 1;
                lowest = slot->priority;
            }
        }
        ++index;
        ++slot;
    } while (index < 32);
    if (chosen == 0 && fallback != 0) chosen = fallback;
    if (chosen == 0) return;
    slot = &data_ov005_0206aa78[chosen - 1];
    slot->state = request | 0x8000;
    slot->priority = priority;
    slot->wait = wait;
    slot->delay = delay;
}

void func_ov005_02069ba0(Overlay5Entry *entry) {
    entry->vtable = data_ov005_02069fe0;
    entry->references = 0;
    entry->key = 0;
    entry->vtable = data_ov005_02069fb0;
    entry->unknown_14 = 0;
}

void func_ov005_02069b7c(void) {
    func_020488bc(&data_ov005_0206aa40, 2, sizeof(Overlay5Entry) + 4, func_ov005_02067638);
}

void func_ov005_02069b54(void) {
    data_ov005_0206aa40.head.previous = 0;
    data_ov005_0206aa40.head.next = &data_ov005_0206aa5c;
    data_ov005_0206aa40.tail.previous = (Overlay5Entry *)&data_ov005_0206aa40;
    data_ov005_0206aa40.tail.next = 0;
}

Overlay5Entry *func_ov005_02069b08(u32 key) {
    Overlay5Entry *entry = data_ov005_0206aa40.head.next;
    while (entry != &data_ov005_0206aa5c) {
        if (entry->key == key) {
            ++entry->references;
            return entry;
        }
        entry = entry->next;
    }
    return 0;
}

void func_ov005_02069acc(Overlay5Entry *entry, u32 key) {
    entry->previous = data_ov005_0206aa40.tail.previous;
    entry->next = &data_ov005_0206aa5c;
    entry->previous->next = entry;
    entry->next->previous = entry;
    entry->key = key;
    entry->references = 1;
}

void func_ov005_02069a54(Overlay5Entry *entry) {
    if (!entry->references) return;
    --entry->references;
    if (entry->references) return;
    entry->next->previous = entry->previous;
    entry->previous->next = entry->next;
    ((Overlay5EntryInterface *)entry)->release();
    if (entry) ((Overlay5EntryInterface *)entry)->destroy();
}

void func_ov005_020699f0(void) {
    Overlay5Entry *entry = data_ov005_0206aa40.head.next;
    while (entry != &data_ov005_0206aa5c) {
        Overlay5Entry *current = entry;
        entry = entry->next;
        ((Overlay5EntryInterface *)current)->finish();
        if (current) ((Overlay5EntryInterface *)current)->destroy();
    }
    func_ov005_02069b54();
}

}
