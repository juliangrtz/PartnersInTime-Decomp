/* Restore the main-screen OBJ graphics after the touchscreen mask-erase
 * effect, release its four loaded images, and retire the effect allocation. */
#include <game/field_system.h>
#include <game/sprite_window.h>
#include <game/sprite_output.h>
#include <game/heap.h>
#include <nitro/gx_init.h>
u32 GX_ResetBankForTex(void);
u32 GX_ResetBankForTexPltt(void);
void func_02035fd0(int);
void FieldSystem_ReleaseMaskErase(FieldSystem *system)
{
    FieldPendingTransfer *transfer = system->pending_transfer;
    if (transfer) {
        int image;
        system->pending_transfer = 0;
        *(vu32 *)0x04000540 = 1;
        GX_ResetBankForTex();
        func_02035fd0(2);
        GX_ResetBankForTexPltt();
        GX_SetGraphicsMode(1, 0, 0);
        *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x1f00;
        GameSprite_CopyBytes(transfer->saved_object_tiles, (void *)0x06400000, 131072);
        system->windows->base.display.bits.fixed_main_scroll = 0;
        for (image = 0; image < 4; ++image) GameHeap_DeleteArray(transfer->images[image]);
        GameHeap_Delete(transfer);
    }
}
