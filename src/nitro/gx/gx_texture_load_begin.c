#include <nitro.h>

extern u8 *data_02062c88;
extern int data_02062c94;
extern u32 data_02062c98;
extern u32 data_02062c9c;
extern const u16 data_0204fad4[][3];
extern const u16 data_0204fad6[][3];
extern const u16 data_0204fad8[][3];
extern int func_02035818(void);

/* The texture mapping table describes two physical spans and the logical
 * offset where a transfer must switch to the second span. */
void func_02038984(void) {
    int banks = func_02035818();
    u32 first = data_0204fad4[banks][0] << 12;
    u32 second = data_0204fad6[banks][0] << 12;
    u32 split = data_0204fad8[banks][0] << 12;
    data_02062c94 = banks;
    data_02062c88 = (u8 *)first;
    data_02062c98 = second;
    data_02062c9c = split;
}

