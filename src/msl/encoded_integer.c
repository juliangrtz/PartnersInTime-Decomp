#include <nitro.h>

typedef struct MslCaughtException {
    void *object;
    u32 unknown04;
    void (*destructor)(void *object, int mode);
} MslCaughtException;

void MSL_DestroyCaughtException(MslCaughtException *exception)
{
    if (!exception->object) {
        return;
    }
    if (!exception->destructor) {
        return;
    }
    exception->destructor(exception->object, -1);
}

const u8 *MSL_DecodeSigned(const u8 *input, s32 *value)
{
    s32 first = (s8)*input;
    u32 second;
    u32 third;
    if (!(first & 1)) {
        *value = first >> 1;
        return input + 1;
    }
    second = input[1];
    if (!(first & 2)) {
        *value = second | ((first >> 2) << 8);
        return input + 2;
    }
    third = input[2];
    if (!(first & 4)) {
        *value = third | (((first >> 3) << 16) | (second << 8));
        return input + 3;
    }
    *value = input[3] | (((first >> 3) << 24) | (second << 16) | (third << 8));
    return input + 4;
}

const u8 *MSL_DecodeUnsigned(const u8 *input, u32 *value)
{
    u32 first = *input;
    u32 second;
    u32 third;
    if (!(first & 1)) {
        *value = first >> 1;
        return input + 1;
    }
    second = input[1];
    if (!(first & 2)) {
        *value = second | ((first >> 2) << 8);
        return input + 2;
    }
    third = input[2];
    if (!(first & 4)) {
        *value = third | (((first >> 3) << 16) | (second << 8));
        return input + 3;
    }
    *value = input[3] | (((first >> 3) << 24) | (second << 16) | (third << 8));
    return input + 4;
}
