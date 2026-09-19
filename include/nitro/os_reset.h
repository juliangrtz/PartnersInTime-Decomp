#ifndef NITRO_OS_RESET_H
#define NITRO_OS_RESET_H

/*
 * Soft reset.
 */

#include <nitro.h>

void OS_InitReset(void);
void OSi_SendResetCommand(u32 command);

#endif
