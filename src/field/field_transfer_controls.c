/*
 * Transfer controls (overlay 0, 0x02066AE4-0x02066B34).
 *
 * Whether a transfer is still preparing, and the resume that lets it continue.
 */

#include <game/field_system.h>
int FieldSystem_IsTransferPreparing(FieldSystem *system) {
    FieldPendingTransfer *transfer = system->pending_transfer;
    return transfer && transfer->flags.phase == 0;
}

void FieldSystem_ResumeTransfer(FieldSystem *system) {
    system->touch.enabled = 1;
    system->pending_transfer->flags.phase = 2;
}
