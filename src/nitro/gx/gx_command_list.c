#include <nitro.h>

typedef struct GxCommandList {
    u8 *command;
    u32 *parameters;
    u32 unknown08[2];
    u32 padding_required;
} GxCommandList;

extern void MI_Copy36B(const void *source, void *destination);
extern void MI_Copy64B(const void *source, void *destination);
extern void func_02038ad4(const void *source, void *destination);
void func_02036a98(GxCommandList *list, int count);

#define GEOMETRY_FIFO ((vu32 *)0x04000400)

void func_02036cc0(const void *matrix) {
    *GEOMETRY_FIFO = 22;
    func_02038ad4(matrix, (void *)GEOMETRY_FIFO);
}

void func_02036ca4(const void *matrix) {
    *GEOMETRY_FIFO = 24;
    func_02038ad4(matrix, (void *)GEOMETRY_FIFO);
}

void func_02036c88(const void *matrix) {
    *GEOMETRY_FIFO = 26;
    MI_Copy36B(matrix, (void *)GEOMETRY_FIFO);
}

void func_02036c64(GxCommandList *list, int opcode) {
    if (opcode != 0 && ((u32)list->command & 3) != 0) {
        list->padding_required = 1;
    }
    *list->command = opcode;
}

void func_02036c48(GxCommandList *list, int opcode, u32 p0) {
    list->padding_required = 0;
    *list->command = opcode;
    list->parameters[0] = p0;
}

void func_02036c24(GxCommandList *list, int opcode, u32 p0, u32 p1) {
    list->padding_required = 0;
    *list->command = opcode;
    list->parameters[0] = p0;
    list->parameters[1] = p1;
}

void func_02036be4(GxCommandList *list, int opcode, u32 p0, u32 p1, u32 p2) {
    list->padding_required = 0;
    *list->command = opcode;
    list->parameters[0] = p0;
    list->parameters[1] = p1;
    list->parameters[2] = p2;
}

void func_02036bb8(GxCommandList *list, const void *matrix) {
    list->padding_required = 0;
    *list->command = 22;
    MI_Copy64B(matrix, list->parameters);
}

void func_02036b8c(GxCommandList *list, const void *matrix) {
    list->padding_required = 0;
    *list->command = 26;
    MI_Copy36B(matrix, list->parameters);
}

void func_02036b6c(GxCommandList *list, int opcode) {
    func_02036c64(list, opcode);
    func_02036a98(list, 0);
}

void func_02036b4c(GxCommandList *list, int opcode, u32 p0) {
    func_02036c48(list, opcode, p0);
    func_02036a98(list, 1);
}

void func_02036b2c(GxCommandList *list, int opcode, u32 p0, u32 p1) {
    func_02036c24(list, opcode, p0, p1);
    func_02036a98(list, 2);
}

void func_02036afc(GxCommandList *list, int opcode, u32 p0, u32 p1, u32 p2) {
    func_02036be4(list, opcode, p0, p1, p2);
    func_02036a98(list, 3);
}

/* Four opcodes share a word. Parameterless commands can require a padding
 * word before the next opcode group. */
void func_02036a98(GxCommandList *list, int count) {
    list->parameters += count;
    ++list->command;
    if (((u32)list->command & 3) == 0) {
        if (list->padding_required) {
            *list->parameters++ = 0;
            list->padding_required = 0;
        }
        list->command = (u8 *)list->parameters++;
    }
}

void func_02036a78(GxCommandList *list, const void *matrix) {
    func_02036bb8(list, matrix);
    func_02036a98(list, 16);
}

void func_02036a58(GxCommandList *list, const void *matrix) {
    func_02036b8c(list, matrix);
    func_02036a98(list, 9);
}
