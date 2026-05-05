import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_MODE,
    CONF_STEP,
    CONF_UNIT_OF_MEASUREMENT,
    ENTITY_CATEGORY_CONFIG,
    ICON_EMPTY,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_VOLT,
)

from .. import CONF_KS_BMS_BLE_ID, KS_BMS_BLE_COMPONENT_SCHEMA, ks_bms_ble_ns

DEPENDENCIES = ["ks_bms_ble"]
CODEOWNERS = ["@syssi"]

UNIT_AMPERE_HOUR = "Ah"
UNIT_MILLISECOND = "ms"

KsNumber = ks_bms_ble_ns.class_("KsNumber", number.Number, cg.Component)

CONF_CELL_FULL_VOLTAGE = "cell_full_voltage"
CONF_CELL_CUTOFF_VOLTAGE = "cell_cutoff_voltage"
CONF_BALANCE_OPEN_VOLTAGE = "balance_open_voltage"
CONF_BALANCE_OPEN_VOLTAGE_DIFF = "balance_open_voltage_diff"
CONF_SOC_80_VOLTAGE = "soc_80_voltage"
CONF_SOC_60_VOLTAGE = "soc_60_voltage"
CONF_SOC_40_VOLTAGE = "soc_40_voltage"
CONF_SOC_20_VOLTAGE = "soc_20_voltage"
CONF_NOMINAL_CAPACITY = "nominal_capacity"
CONF_CYCLE_CAPACITY = "cycle_capacity"
CONF_FULL_CHARGE_CAPACITY = "full_charge_capacity"
CONF_CELL_OVERVOLTAGE_PROTECTION = "cell_overvoltage_protection"
CONF_CELL_OVERVOLTAGE_RECOVERY = "cell_overvoltage_recovery"
CONF_CELL_OVERVOLTAGE_PROTECTION_DELAY = "cell_overvoltage_protection_delay"
CONF_CELL_UNDERVOLTAGE_PROTECTION = "cell_undervoltage_protection"
CONF_CELL_UNDERVOLTAGE_RECOVERY = "cell_undervoltage_recovery"
CONF_CELL_UNDERVOLTAGE_PROTECTION_DELAY = "cell_undervoltage_protection_delay"
CONF_PACK_OVERVOLTAGE_PROTECTION = "pack_overvoltage_protection"
CONF_PACK_OVERVOLTAGE_RECOVERY = "pack_overvoltage_recovery"
CONF_PACK_OVERVOLTAGE_PROTECTION_DELAY = "pack_overvoltage_protection_delay"
CONF_PACK_UNDERVOLTAGE_PROTECTION = "pack_undervoltage_protection"
CONF_PACK_UNDERVOLTAGE_RECOVERY = "pack_undervoltage_recovery"
CONF_PACK_UNDERVOLTAGE_PROTECTION_DELAY = "pack_undervoltage_protection_delay"
CONF_CHARGE_OVERTEMPERATURE_PROTECTION = "charge_overtemperature_protection"
CONF_CHARGE_OVERTEMPERATURE_RECOVERY = "charge_overtemperature_recovery"
CONF_CHARGE_OVERTEMPERATURE_PROTECTION_DELAY = "charge_overtemperature_protection_delay"
CONF_CHARGE_UNDERTEMPERATURE_PROTECTION_DELAY = (
    "charge_undertemperature_protection_delay"
)
CONF_CHARGE_UNDERTEMPERATURE_RECOVERY = "charge_undertemperature_recovery"
CONF_CHARGE_UNDERTEMPERATURE_PROTECTION = "charge_undertemperature_protection"
CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION = "discharge_overtemperature_protection"
CONF_DISCHARGE_OVERTEMPERATURE_RECOVERY = "discharge_overtemperature_recovery"
CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION_DELAY = (
    "discharge_overtemperature_protection_delay"
)
CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION = "discharge_undertemperature_protection"
CONF_DISCHARGE_UNDERTEMPERATURE_RECOVERY = "discharge_undertemperature_recovery"
CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION_DELAY = (
    "discharge_undertemperature_protection_delay"
)
CONF_CHARGE_OVERCURRENT_PROTECTION = "charge_overcurrent_protection"
CONF_CHARGE_OVERCURRENT_PROTECTION_DELAY = "charge_overcurrent_protection_delay"
CONF_CHARGE_OVERCURRENT_RECOVERY_DELAY = "charge_overcurrent_recovery_delay"
CONF_DISCHARGE_OVERCURRENT_PROTECTION = "discharge_overcurrent_protection"
CONF_DISCHARGE_OVERCURRENT_PROTECTION_DELAY = "discharge_overcurrent_protection_delay"
CONF_DISCHARGE_OVERCURRENT_RECOVERY_DELAY = "discharge_overcurrent_recovery_delay"

# [register, factor, offset]
# Temperature encoding: encoded = round(value_°C * 10 + 2731)  →  factor=10, offset=2731
# Delay encoding (*1000): encoded = round(value_ms * 1)  →  factor=1, 1 ms resolution
# Delay encoding (*100):  encoded = round(value_ms * 0.1)  →  factor=0.1, 10 ms resolution
NUMBERS = {
    CONF_CELL_FULL_VOLTAGE: [0x10, 1000.0, 0.0],
    CONF_CELL_CUTOFF_VOLTAGE: [0x11, 1000.0, 0.0],
    CONF_BALANCE_OPEN_VOLTAGE: [0x12, 1000.0, 0.0],
    CONF_BALANCE_OPEN_VOLTAGE_DIFF: [0x13, 1000.0, 0.0],
    CONF_SOC_80_VOLTAGE: [0x16, 1000.0, 0.0],
    CONF_SOC_60_VOLTAGE: [0x17, 1000.0, 0.0],
    CONF_SOC_40_VOLTAGE: [0x18, 1000.0, 0.0],
    CONF_SOC_20_VOLTAGE: [0x19, 1000.0, 0.0],
    CONF_NOMINAL_CAPACITY: [0x1A, 100.0, 0.0],
    CONF_CYCLE_CAPACITY: [0x1B, 100.0, 0.0],
    CONF_FULL_CHARGE_CAPACITY: [0x1C, 100.0, 0.0],
    CONF_CELL_OVERVOLTAGE_PROTECTION: [0x20, 1000.0, 0.0],
    CONF_CELL_OVERVOLTAGE_RECOVERY: [0x21, 1000.0, 0.0],
    CONF_CELL_OVERVOLTAGE_PROTECTION_DELAY: [0x22, 1.0, 0.0],
    CONF_CELL_UNDERVOLTAGE_PROTECTION: [0x23, 1000.0, 0.0],
    CONF_CELL_UNDERVOLTAGE_RECOVERY: [0x24, 1000.0, 0.0],
    CONF_CELL_UNDERVOLTAGE_PROTECTION_DELAY: [0x25, 1.0, 0.0],
    CONF_PACK_OVERVOLTAGE_PROTECTION: [0x26, 100.0, 0.0],
    CONF_PACK_OVERVOLTAGE_RECOVERY: [0x27, 100.0, 0.0],
    CONF_PACK_OVERVOLTAGE_PROTECTION_DELAY: [0x28, 1.0, 0.0],
    CONF_PACK_UNDERVOLTAGE_PROTECTION: [0x29, 100.0, 0.0],
    CONF_PACK_UNDERVOLTAGE_RECOVERY: [0x2A, 100.0, 0.0],
    CONF_PACK_UNDERVOLTAGE_PROTECTION_DELAY: [0x2B, 1.0, 0.0],
    CONF_CHARGE_OVERTEMPERATURE_PROTECTION: [0x30, 10.0, 2731.0],
    CONF_CHARGE_OVERTEMPERATURE_RECOVERY: [0x31, 10.0, 2731.0],
    CONF_CHARGE_OVERTEMPERATURE_PROTECTION_DELAY: [0x32, 1.0, 0.0],
    CONF_CHARGE_UNDERTEMPERATURE_PROTECTION: [0x33, 10.0, 2731.0],
    CONF_CHARGE_UNDERTEMPERATURE_RECOVERY: [0x34, 10.0, 2731.0],
    CONF_CHARGE_UNDERTEMPERATURE_PROTECTION_DELAY: [0x35, 1.0, 0.0],
    CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION: [0x36, 10.0, 2731.0],
    CONF_DISCHARGE_OVERTEMPERATURE_RECOVERY: [0x37, 10.0, 2731.0],
    CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION_DELAY: [0x38, 1.0, 0.0],
    CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION: [0x39, 10.0, 2731.0],
    CONF_DISCHARGE_UNDERTEMPERATURE_RECOVERY: [0x3A, 10.0, 2731.0],
    CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION_DELAY: [0x3B, 1.0, 0.0],
    CONF_CHARGE_OVERCURRENT_PROTECTION: [0x40, 100.0, 0.0],
    CONF_CHARGE_OVERCURRENT_PROTECTION_DELAY: [0x41, 1.0, 0.0],
    CONF_CHARGE_OVERCURRENT_RECOVERY_DELAY: [0x42, 0.1, 0.0],
    CONF_DISCHARGE_OVERCURRENT_PROTECTION: [0x43, 100.0, 0.0],
    CONF_DISCHARGE_OVERCURRENT_PROTECTION_DELAY: [0x44, 1.0, 0.0],
    CONF_DISCHARGE_OVERCURRENT_RECOVERY_DELAY: [0x45, 0.1, 0.0],
}

KS_NUMBER_SCHEMA = (
    number.number_schema(
        KsNumber,
        icon=ICON_EMPTY,
        entity_category=ENTITY_CATEGORY_CONFIG,
        unit_of_measurement=UNIT_VOLT,
    )
    .extend(
        {
            cv.Optional(CONF_STEP, default=0.001): cv.float_,
            cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                number.NUMBER_MODES, upper=True
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = KS_BMS_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_CELL_FULL_VOLTAGE): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=3.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_CELL_CUTOFF_VOLTAGE): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_BALANCE_OPEN_VOLTAGE): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_BALANCE_OPEN_VOLTAGE_DIFF): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.001): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_SOC_80_VOLTAGE): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_SOC_60_VOLTAGE): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_SOC_40_VOLTAGE): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_SOC_20_VOLTAGE): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_NOMINAL_CAPACITY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE_HOUR
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CYCLE_CAPACITY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE_HOUR
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_FULL_CHARGE_CAPACITY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE_HOUR
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CELL_OVERVOLTAGE_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=3.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_CELL_OVERVOLTAGE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=3.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.5): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_CELL_OVERVOLTAGE_PROTECTION_DELAY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CELL_UNDERVOLTAGE_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_CELL_UNDERVOLTAGE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=2.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=4.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.001): cv.float_,
            }
        ),
        cv.Optional(CONF_CELL_UNDERVOLTAGE_PROTECTION_DELAY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_PACK_OVERVOLTAGE_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=10.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_PACK_OVERVOLTAGE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=10.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_PACK_OVERVOLTAGE_PROTECTION_DELAY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_PACK_UNDERVOLTAGE_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=10.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_PACK_UNDERVOLTAGE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=10.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_PACK_UNDERVOLTAGE_PROTECTION_DELAY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CHARGE_OVERTEMPERATURE_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CHARGE_OVERTEMPERATURE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(
            CONF_CHARGE_OVERTEMPERATURE_PROTECTION_DELAY
        ): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CHARGE_UNDERTEMPERATURE_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CHARGE_UNDERTEMPERATURE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(
            CONF_CHARGE_UNDERTEMPERATURE_PROTECTION_DELAY
        ): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_DISCHARGE_OVERTEMPERATURE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(
            CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION_DELAY
        ): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(
            CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION
        ): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_DISCHARGE_UNDERTEMPERATURE_RECOVERY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=-40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_CELSIUS
                ): cv.string_strict,
            }
        ),
        cv.Optional(
            CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION_DELAY
        ): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CHARGE_OVERCURRENT_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CHARGE_OVERCURRENT_PROTECTION_DELAY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_CHARGE_OVERCURRENT_RECOVERY_DELAY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655000.0): cv.float_,
                cv.Optional(CONF_STEP, default=10.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_DISCHARGE_OVERCURRENT_PROTECTION): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE
                ): cv.string_strict,
            }
        ),
        cv.Optional(
            CONF_DISCHARGE_OVERCURRENT_PROTECTION_DELAY
        ): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=65000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_DISCHARGE_OVERCURRENT_RECOVERY_DELAY): KS_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=655000.0): cv.float_,
                cv.Optional(CONF_STEP, default=10.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MILLISECOND
                ): cv.string_strict,
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_KS_BMS_BLE_ID])
    for key, address in NUMBERS.items():
        if key in config:
            conf = config[key]
            var = await number.new_number(
                conf,
                min_value=conf[CONF_MIN_VALUE],
                max_value=conf[CONF_MAX_VALUE],
                step=conf[CONF_STEP],
            )
            await cg.register_component(var, conf)
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address[0]))
            cg.add(var.set_factor(address[1]))
            cg.add(var.set_offset(address[2]))
            cg.add(getattr(hub, f"set_{key}_number")(var))
