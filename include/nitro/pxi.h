#ifndef NITRO_PXI_H
#define NITRO_PXI_H

/*
 * The FIFO between the two processors and the per-tag receive callbacks that
 * route an incoming word to its subsystem.
 */

#include <nitro.h>

typedef void (*PxiCallback)(u32 tag, u32 data, int error);

void PXI_Init(void);
int PXI_IsCallbackReady(u32 tag, int processor);
void PXI_SetFifoRecvCallback(u32 tag, PxiCallback callback);
int PXI_SendWordByFifo(u32 tag, u32 data, int error);

#endif
