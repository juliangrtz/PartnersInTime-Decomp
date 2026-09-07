#include <nitro.h>

typedef struct CpContext {
    u64 numerator;
    u64 denominator;
    u64 square_root;
    u16 division_mode;
    u16 square_root_mode;
} CpContext;

void CP_SaveContext(CpContext *context) {
    context->numerator = *(volatile u64 *)0x04000290;
    context->denominator = *(volatile u64 *)0x04000298;
    context->division_mode = *(vu16 *)0x04000280 & 3;
    context->square_root = *(volatile u64 *)0x040002B8;
    context->square_root_mode = *(vu16 *)0x040002B0 & 1;
}

void CP_RestoreContext(const CpContext *context) {
    *(volatile u64 *)0x04000290 = context->numerator;
    *(volatile u64 *)0x04000298 = context->denominator;
    *(vu16 *)0x04000280 = context->division_mode;
    *(volatile u64 *)0x040002B8 = context->square_root;
    *(vu16 *)0x040002B0 = context->square_root_mode;
}
