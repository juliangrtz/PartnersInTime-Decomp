#include <game/overlay017_attack.h>

enum Overlay17AttackTuningConstant {
    OVERLAY17_TUNING_ROW_COUNT = 30,
    OVERLAY17_TUNING_REVERSE_START = OVERLAY17_TUNING_ROW_COUNT - 2,
    OVERLAY17_TUNING_SPLIT_ROW = OVERLAY17_TUNING_ROW_COUNT / 2,
    OVERLAY17_PAIR_TUNING_COLUMN_COUNT = 3,
    OVERLAY17_GROUP_TUNING_COLUMN_COUNT = 5,
};

typedef struct Overlay17PairTuningRow {
    u16 column[OVERLAY17_PAIR_TUNING_COLUMN_COUNT];
} Overlay17PairTuningRow;

typedef struct Overlay17GroupTuningRow {
    u16 column[OVERLAY17_GROUP_TUNING_COLUMN_COUNT];
} Overlay17GroupTuningRow;

extern Overlay17PairTuningRow data_ov017_020c6cf0[];
extern u16 data_ov017_020c6d9e[];
extern Overlay17GroupTuningRow data_ov017_020c6da4[];
extern u16 data_ov017_020c6ec6[];

/* Metrowerks emits C functions in reverse source order. */
u16 Overlay17Attack_GetGroupTuningParameter1(int value) {
    return Overlay17Attack_LookupGroupTuning(value, 1);
}

u16 Overlay17Attack_GetGroupTuningParameter2(int value) {
    return Overlay17Attack_LookupGroupTuning(value, 2);
}

u16 Overlay17Attack_GetGroupTuningParameter3(int value) {
    return Overlay17Attack_LookupGroupTuning(value, 3);
}

u16 Overlay17Attack_GetGroupTuningParameter4(int value) {
    return Overlay17Attack_LookupGroupTuning(value, 4);
}

u16 Overlay17Attack_GetPairTuningParameter1(int value) {
    return Overlay17Attack_LookupPairTuning(value, 1);
}

u16 Overlay17Attack_GetPairTuningParameter2(int value) {
    return Overlay17Attack_LookupPairTuning(value, 2);
}

u16 Overlay17Attack_LookupGroupTuning(int value, int parameter_index) {
    u32 row;
    Overlay17GroupTuningRow *entry = data_ov017_020c6da4;

    if (value < data_ov017_020c6da4[OVERLAY17_TUNING_SPLIT_ROW].column[0]) {
        for (row = 0; row < OVERLAY17_TUNING_ROW_COUNT; ++row) {
            if (value <= entry->column[0]) {
                return data_ov017_020c6da4[row].column[parameter_index];
            }
            ++entry;
        }
        return data_ov017_020c6ec6[parameter_index];
    } else {
        int reverse_row;

        entry = &data_ov017_020c6da4[OVERLAY17_TUNING_REVERSE_START];
        for (reverse_row = OVERLAY17_TUNING_REVERSE_START;
             reverse_row >= 0;
             --reverse_row) {
            if (value > entry->column[0]) {
                return data_ov017_020c6da4[reverse_row + 1]
                    .column[parameter_index];
            }
            --entry;
        }
        return data_ov017_020c6da4[0].column[parameter_index];
    }
}

u16 Overlay17Attack_LookupPairTuning(int value, int parameter_index) {
    u32 row;
    Overlay17PairTuningRow *entry = data_ov017_020c6cf0;

    if (value < data_ov017_020c6cf0[OVERLAY17_TUNING_SPLIT_ROW].column[0]) {
        for (row = 0; row < OVERLAY17_TUNING_ROW_COUNT; ++row) {
            if (value <= entry->column[0]) {
                return data_ov017_020c6cf0[row].column[parameter_index];
            }
            ++entry;
        }
        return data_ov017_020c6d9e[parameter_index];
    } else {
        int reverse_row;

        entry = &data_ov017_020c6cf0[OVERLAY17_TUNING_REVERSE_START];
        for (reverse_row = OVERLAY17_TUNING_REVERSE_START;
             reverse_row >= 0;
             --reverse_row) {
            if (value > entry->column[0]) {
                return data_ov017_020c6cf0[reverse_row + 1]
                    .column[parameter_index];
            }
            --entry;
        }
        return data_ov017_020c6cf0[0].column[parameter_index];
    }
}
