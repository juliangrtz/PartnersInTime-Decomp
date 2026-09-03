from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import reassembly  # noqa: E402
import relink_arm7  # noqa: E402
import relink_overlay  # noqa: E402


class Arm7LayoutTests(unittest.TestCase):
    def test_resolves_arm_calls_and_literal_loads(self) -> None:
        self.assertEqual(
            relink_arm7.resolve_arm_relocation(0xEB00003E, 0x1000, "arm_call"),
            0x1100,
        )
        self.assertEqual(
            relink_arm7.resolve_arm_relocation(0xE59F0010, 0x2000, "load"),
            0x2018,
        )
        self.assertEqual(
            relink_arm7.resolve_arm_relocation(0xEA000000, 0x3000, "arm_branch"),
            0x3008,
        )
        self.assertEqual(
            relink_arm7.resolve_arm_relocation(0x02380170, 0x4000, "data"),
            0x02380170,
        )
        with self.assertRaises(reassembly.ReassemblyError):
            relink_arm7.resolve_arm_relocation(0xE1A00000, 0x2000, "load")

    def test_arm7_symbol_map_is_cpu_specific(self) -> None:
        symbols = relink_overlay.read_all_symbols("eur", "arm7")
        self.assertEqual(symbols["ARM7_Entry"], 0x02380000)
        self.assertEqual(symbols["SVC_Halt_Thunk"], 0x037F8524)
        self.assertEqual(symbols["SVC_Halt"], 0x03803DAE)
        self.assertEqual(symbols["SVC_WaitByLoop"], 0x03803D94)
        self.assertEqual(symbols["OSi_IrqVBlank"], 0x037FB5C8)
        self.assertEqual(symbols["OSi_IrqTimer3"], 0x037FB620)
        self.assertEqual(symbols["OSi_IrqTimer0"], 0x037FB650)
        self.assertEqual(symbols["OSi_IrqDma3"], 0x037FB660)
        self.assertEqual(symbols["OSi_IrqDma0"], 0x037FB690)
        self.assertEqual(symbols["OSi_IrqCallback"], 0x037FB6A0)
        self.assertEqual(symbols["OS_IrqDummy"], 0x037FB734)
        self.assertEqual(symbols["OS_ResetRequestIrqMask"], 0x037FB738)
        self.assertEqual(symbols["OS_DisableIrqMask"], 0x037FB76C)
        self.assertEqual(symbols["OS_EnableIrqMask"], 0x037FB7A8)
        self.assertEqual(symbols["OS_SetIrqMask"], 0x037FB7E0)
        self.assertEqual(symbols["OS_SetIrqFunction"], 0x037FB860)
        self.assertEqual(symbols["OSi_SetTimerCallback"], 0x037FB814)
        self.assertEqual(symbols["OS_InitIrqTable"], 0x037FB908)
        self.assertEqual(symbols["OS_UnLockCartridge"], 0x037FB928)
        self.assertEqual(symbols["OS_GetLockID"], 0x037FB934)
        self.assertEqual(symbols["OS_ReleaseLockID"], 0x037FB9CC)
        self.assertEqual(symbols["OS_ReadOwnerOfLockWord"], 0x037FBA0C)
        self.assertEqual(symbols["OS_TryLockCartridge"], 0x037FBA1C)
        self.assertEqual(symbols["OS_UnlockCartridge"], 0x037FBA3C)
        self.assertEqual(symbols["OS_LockCartridge"], 0x037FBA5C)
        self.assertEqual(symbols["OSi_DoTryLockByWord"], 0x037FBA7C)
        self.assertEqual(symbols["OSi_DoUnlockByWord"], 0x037FBB00)
        self.assertEqual(symbols["OSi_DoLockByWord"], 0x037FBB8C)
        self.assertEqual(symbols["OS_InitLock"], 0x037FBBD4)
        self.assertEqual(symbols["OS_EnableScheduler"], 0x037FBC50)
        self.assertEqual(symbols["OS_DisableScheduler"], 0x037FBC88)
        self.assertEqual(symbols["OS_IrqHandler"], 0x037FB458)
        self.assertEqual(symbols["OS_SetSwitchThreadCallback"], 0x037FBCC0)
        self.assertEqual(symbols["OSi_SleepAlarmCallback"], 0x037FBCF4)
        self.assertEqual(symbols["OS_Sleep"], 0x037FBD00)
        self.assertEqual(symbols["OS_SetThreadPriority"], 0x037FBD70)
        self.assertEqual(symbols["OSi_RescheduleThread"], 0x037FBE20)
        self.assertEqual(symbols["OS_SelectThread"], 0x037FBEF4)
        self.assertEqual(symbols["OS_WakeupThreadDirect"], 0x037FBF20)
        self.assertEqual(symbols["OS_WakeupThread"], 0x037FBF54)
        self.assertEqual(symbols["OS_SleepThread"], 0x037FBFC4)
        self.assertEqual(symbols["OS_ExitThread"], 0x037FC02C)
        self.assertEqual(symbols["OS_CreateThread"], 0x037FC088)
        self.assertEqual(symbols["OS_InitThread"], 0x037FC188)
        self.assertEqual(symbols["OSi_RemoveThreadFromList"], 0x037FC2B4)
        self.assertEqual(symbols["OSi_InsertThreadToList"], 0x037FC2FC)
        self.assertEqual(symbols["OSi_GetUnusedThreadId"], 0x037FC364)
        self.assertEqual(symbols["OS_InitContext"], 0x037FC398)
        self.assertEqual(symbols["OS_SaveContext"], 0x037FC3FC)
        self.assertEqual(symbols["OS_LoadContext"], 0x037FC430)
        self.assertEqual(symbols["OS_ReadMessage"], 0x037FC45C)
        self.assertEqual(symbols["OS_ReceiveMessage"], 0x037FC4D4)
        self.assertEqual(symbols["OS_SendMessage"], 0x037FC574)
        self.assertEqual(symbols["OS_InitMessageQueue"], 0x037FC608)
        self.assertEqual(symbols["OSi_UnlockAllMutex"], 0x037FC698)
        self.assertEqual(symbols["OS_UnlockMutex"], 0x037FC6D8)
        self.assertEqual(symbols["OS_LockMutex"], 0x037FC748)
        self.assertEqual(symbols["OS_InitMutex"], 0x037FC7D4)
        self.assertEqual(symbols["OS_Init"], 0x037FC7E8)
        self.assertEqual(symbols["OS_SetArenaLo"], 0x037FC820)
        self.assertEqual(symbols["OS_SetArenaHi"], 0x037FC834)
        self.assertEqual(symbols["OSi_GetInitArenaLo"], 0x037FC848)
        self.assertEqual(symbols["OSi_GetInitArenaHi"], 0x037FC8A0)
        self.assertEqual(symbols["OS_GetArenaLo"], 0x037FC910)
        self.assertEqual(symbols["OS_GetArenaHi"], 0x037FC924)
        self.assertEqual(symbols["OS_InitArena"], 0x037FC938)
        self.assertEqual(symbols["OSi_SetTimerReserved"], 0x037FCF90)
        self.assertEqual(symbols["OS_GetTick"], 0x037FCFAC)
        self.assertEqual(symbols["OSi_CountUpTick"], 0x037FD05C)
        self.assertEqual(symbols["OS_IsTickAvailable"], 0x037FD0E4)
        self.assertEqual(symbols["OS_InitTick"], 0x037FD0F4)
        self.assertEqual(symbols["OSi_AlarmHandler"], 0x037FD188)
        self.assertEqual(symbols["OSi_ArrangeTimer"], 0x037FD198)
        self.assertEqual(symbols["OS_CancelAlarm"], 0x037FD298)
        self.assertEqual(symbols["OS_SetPeriodicAlarm"], 0x037FD328)
        self.assertEqual(symbols["OS_SetAlarm"], 0x037FD3A4)
        self.assertEqual(symbols["OSi_InsertAlarm"], 0x037FD418)
        self.assertEqual(symbols["OS_CreateAlarm"], 0x037FD548)
        self.assertEqual(symbols["OS_IsAlarmAvailable"], 0x037FD558)
        self.assertEqual(symbols["OS_InitAlarm"], 0x037FD568)
        self.assertEqual(symbols["OSi_SetTimer"], 0x037FD5B8)
        self.assertEqual(symbols["OS_EnableInterrupts"], 0x037FDC90)
        self.assertEqual(symbols["OS_DisableInterrupts"], 0x037FDCA4)
        self.assertEqual(symbols["OS_RestoreInterrupts"], 0x037FDCB8)
        self.assertEqual(symbols["OS_ResetSystem"], 0x037FDD48)
        self.assertEqual(symbols["OS_IsResetOccurred"], 0x037FDE08)
        self.assertEqual(symbols["OS_InitReset"], 0x037FDE18)
        self.assertEqual(symbols["MI_StopDma"], 0x037FDE58)
        self.assertEqual(symbols["MI_SwapWord"], 0x037FE1BC)
        self.assertEqual(symbols["PXI_Init"], 0x037FE1C4)
        self.assertEqual(symbols["PXIi_HandlerRecvFifoNotEmpty"], 0x037FE1D0)
        self.assertEqual(symbols["PXI_SendWordByFifo"], 0x037FE2F0)
        self.assertEqual(symbols["PXI_IsCallbackReady"], 0x037FE394)
        self.assertEqual(symbols["PXI_SetFifoRecvCallback"], 0x037FE3BC)
        self.assertEqual(symbols["PXIi_InitFifo"], 0x037FE428)
        self.assertEqual(symbols["SndThread"], 0x037FEEC4)
        self.assertEqual(symbols["SndAlarmCallback"], 0x037FEF44)
        self.assertEqual(symbols["SND_SendWakeupMessage"], 0x037FEF98)
        self.assertEqual(symbols["SND_WaitForIntervalTimer"], 0x037FEFB4)
        self.assertEqual(symbols["SND_StopIntervalTimer"], 0x037FEFE0)
        self.assertEqual(symbols["SND_StartIntervalTimer"], 0x037FEFF4)
        self.assertEqual(symbols["SND_InitIntervalTimer"], 0x037FF04C)
        self.assertEqual(symbols["SND_CreateThread"], 0x037FF084)
        self.assertEqual(symbols["SND_Init"], 0x037FF0CC)
        self.assertEqual(symbols["SND_CommandInit"], 0x03802A7C)
        self.assertEqual(symbols["CTRDG_Init"], 0x03806D04)
        self.assertEqual(symbols["CTRDGi_Initialized"], 0x0380B1B0)
        self.assertEqual(symbols["OS_IRQTable"], 0x0380794C)
        self.assertEqual(symbols["OS_IRQCheckFlag"], 0x03807A84)
        self.assertEqual(symbols["OSi_IrqCallbackInfoIndex"], 0x03807938)
        self.assertEqual(symbols["OSi_IrqCallbackInfo"], 0x03807A88)
        self.assertEqual(symbols["OSi_VBlankCallbackInfo"], 0x03807AE8)
        self.assertEqual(symbols["OSi_TimerCallbackInfo"], 0x03807AB8)
        self.assertEqual(symbols["OSi_RescheduleCount"], 0x03807AF8)
        self.assertEqual(symbols["OSi_SystemCallbackInSwitchThread"], 0x03807B04)
        self.assertEqual(symbols["OSi_ThreadInfo"], 0x03807B08)
        self.assertEqual(symbols["OSi_ThreadTable"], 0x03807B1C)
        self.assertEqual(symbols["OSi_IdleThread"], 0x03807B5C)
        self.assertEqual(symbols["OSi_MainThread"], 0x03807BD4)
        self.assertEqual(symbols["OSi_ResetInitialized"], 0x03807CA8)
        self.assertEqual(symbols["OSi_IsResetOccurred"], 0x03807CAC)
        self.assertEqual(symbols["OSi_ArenaInitialized"], 0x03807C4C)
        self.assertEqual(symbols["PXIi_FifoInitialized"], 0x03807CB0)
        self.assertEqual(symbols["PXIi_FifoRecvCallbackTable"], 0x03807CB4)
        self.assertEqual(symbols["sSndMessageQueue"], 0x03807D8C)
        self.assertEqual(symbols["sSndIntervalAlarm"], 0x03807DC0)
        self.assertEqual(symbols["sSndThread"], 0x03807DEC)
        self.assertEqual(symbols["sSndThreadStackTop"], 0x03808264)
        self.assertEqual(symbols["ARM7_Autoload000_End"], 0x0380B1CC)
        self.assertEqual(symbols["ARM7_Autoload001_End"], 0x027F8BD0)
        self.assertEqual(symbols["HW_THREADINFO_SUBP"], 0x027FFFA4)
        self.assertEqual(symbols["HW_VBLANK_COUNT_BUF"], 0x027FFC3C)
        self.assertEqual(symbols["HW_LOCK_ID_FLAG_SUBP"], 0x027FFFB8)
        self.assertEqual(symbols["HW_LOCK_CARTRIDGE"], 0x027FFFE8)
        self.assertEqual(symbols["HW_INTR_CHECK_BUF"], 0x0380FFF8)
        self.assertEqual(symbols["REG_DMA0SAD"], 0x040000B0)
        self.assertEqual(symbols["REG_TM1CNT_H"], 0x04000106)
        self.assertEqual(symbols["REG_TM1CNT_L"], 0x04000104)
        self.assertEqual(symbols["REG_PXI_SEND_FIFO"], 0x04000188)

    def synthetic_layout(self) -> tuple[bytes, reassembly.Module]:
        load_address = 0x02380000
        rom_offset = 0x20
        payload_size = 0x90
        data = bytearray(rom_offset + payload_size)
        container = reassembly.Module(
            "arm7", "arm7", rom_offset, payload_size, load_address
        )

        resident_size = 0x30
        first_size = 0x20
        second_size = 0x28
        table_address = load_address + resident_size + first_size + second_size
        struct.pack_into(
            "<6I",
            data,
            rom_offset + 0x10,
            table_address,
            load_address + payload_size,
            load_address + resident_size,
            load_address + resident_size,
            load_address + resident_size,
            0,
        )
        table_offset = rom_offset + table_address - load_address
        struct.pack_into("<3I", data, table_offset, 0x037F8000, first_size, 0x10)
        struct.pack_into(
            "<3I", data, table_offset + 12, 0x027E0000, second_size, 0x18
        )
        return bytes(data), container

    def test_parses_resident_and_autoload_components(self) -> None:
        data, container = self.synthetic_layout()
        layout = relink_arm7.parse_layout_candidate(data, container, 0x10)
        self.assertIsNotNone(layout)
        assert layout is not None
        self.assertEqual(layout.module_parameters_address, 0x02380010)
        self.assertEqual(layout.resident.size, 0x30)
        self.assertEqual(
            [module.name for module in layout.autoloads],
            ["arm7_autoload_000", "arm7_autoload_001"],
        )
        self.assertEqual(
            [module.rom_offset for module in layout.autoloads], [0x50, 0x70]
        )
        self.assertEqual(
            [module.load_address for module in layout.autoloads],
            [0x037F8000, 0x027E0000],
        )
        self.assertEqual([module.bss_size for module in layout.autoloads], [0x10, 0x18])

    def test_rejects_descriptor_sizes_that_do_not_cover_payload(self) -> None:
        data, container = self.synthetic_layout()
        corrupted = bytearray(data)
        struct.pack_into("<I", corrupted, 0x98 + 4, 0x1C)
        self.assertIsNone(
            relink_arm7.parse_layout_candidate(bytes(corrupted), container, 0x10)
        )


if __name__ == "__main__":
    unittest.main()
