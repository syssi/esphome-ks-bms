import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_CURRENT,
    CONF_POWER,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_EMPTY,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_WATT,
)

from . import CONF_KS_BMS_BLE_ID, KS_BMS_BLE_COMPONENT_SCHEMA

DEPENDENCIES = ["ks_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_TOTAL_VOLTAGE = "total_voltage"
# CONF_CURRENT = "current"
# CONF_POWER = "power"
CONF_CHARGING_POWER = "charging_power"
CONF_DISCHARGING_POWER = "discharging_power"
CONF_CAPACITY_REMAINING = "capacity_remaining"

CONF_VOLTAGE_PROTECTION_BITMASK = "voltage_protection_bitmask"
CONF_CURRENT_PROTECTION_BITMASK = "current_protection_bitmask"
CONF_TEMPERATURE_PROTECTION_BITMASK = "temperature_protection_bitmask"
CONF_ERROR_BITMASK = "error_bitmask"
CONF_STATE_OF_CHARGE = "state_of_charge"
CONF_NOMINAL_CAPACITY = "nominal_capacity"
CONF_FULL_CHARGE_CAPACITY = "full_charge_capacity"
CONF_CHARGING_CYCLES = "charging_cycles"
CONF_MIN_CELL_VOLTAGE = "min_cell_voltage"
CONF_MAX_CELL_VOLTAGE = "max_cell_voltage"
CONF_MIN_VOLTAGE_CELL = "min_voltage_cell"
CONF_MAX_VOLTAGE_CELL = "max_voltage_cell"
CONF_DELTA_CELL_VOLTAGE = "delta_cell_voltage"
CONF_AVERAGE_CELL_VOLTAGE = "average_cell_voltage"
CONF_STATE_OF_HEALTH = "state_of_health"
CONF_BALANCED_CELL = "balanced_cell"
CONF_BALANCED_CELL_BITMASK = "balanced_cell_bitmask"

CONF_SHORT_CIRCUIT_PROTECTION_COUNT = "short_circuit_protection_count"
CONF_CHARGE_OVERCURRENT_PROTECTION_COUNT = "charge_overcurrent_protection_count"
CONF_DISCHARGE_OVERCURRENT_PROTECTION_COUNT = "discharge_overcurrent_protection_count"
CONF_CELL_OVERVOLTAGE_PROTECTION_COUNT = "cell_overvoltage_protection_count"
CONF_CHARGE_UNDERCURRENT_PROTECTION_COUNT = "charge_undercurrent_protection_count"
CONF_PACK_OVERVOLTAGE_PROTECTION_COUNT = "pack_overvoltage_protection_count"
CONF_PACK_UNDERVOLTAGE_PROTECTION_COUNT = "pack_undervoltage_protection_count"
CONF_CHARGE_OVERTEMPERATURE_PROTECTION_COUNT = "charge_overtemperature_protection_count"
CONF_CHARGE_UNDERTEMPERATURE_PROTECTION_COUNT = (
    "charge_undertemperature_protection_count"
)
CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION_COUNT = (
    "discharge_overtemperature_protection_count"
)
CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION_COUNT = (
    "discharge_undertemperature_protection_count"
)

CONF_AVERAGE_TEMPERATURE = "average_temperature"
CONF_AMBIENT_TEMPERATURE = "ambient_temperature"
CONF_MOSFET_TEMPERATURE = "mosfet_temperature"

ICON_CURRENT_DC = "mdi:current-dc"
ICON_CAPACITY_REMAINING = "mdi:battery-50"
ICON_NOMINAL_CAPACITY = "mdi:battery-50"
ICON_FULL_CHARGE_CAPACITY = "mdi:battery-charging-100"
ICON_CHARGING_CYCLES = "mdi:battery-sync"
ICON_MIN_CELL_VOLTAGE = "mdi:battery-minus-outline"
ICON_MAX_CELL_VOLTAGE = "mdi:battery-plus-outline"
ICON_MIN_VOLTAGE_CELL = "mdi:battery-minus-outline"
ICON_MAX_VOLTAGE_CELL = "mdi:battery-plus-outline"
ICON_VOLTAGE_PROTECTION_BITMASK = "mdi:alert-circle-outline"
ICON_CURRENT_PROTECTION_BITMASK = "mdi:alert-circle-outline"
ICON_TEMPERATURE_PROTECTION_BITMASK = "mdi:alert-circle-outline"
ICON_ERROR_BITMASK = "mdi:alert-circle-outline"
ICON_STATE_OF_HEALTH = "mdi:heart-flash"
ICON_BALANCED_CELL = "mdi:battery-sync"
ICON_BALANCED_CELL_BITMASK = "mdi:battery-sync-outline"
ICON_PROTECTION_COUNT = "mdi:counter"

UNIT_AMPERE_HOURS = "Ah"

CELLS = [f"cell_voltage_{i}" for i in range(1, 25)]
TEMPERATURES = [f"temperature_{i}" for i in range(1, 9)]

# key: sensor_schema kwargs
SENSOR_DEFS = {
    CONF_TOTAL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_DC,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CHARGING_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DISCHARGING_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CAPACITY_REMAINING: {
        "unit_of_measurement": UNIT_AMPERE_HOURS,
        "icon": ICON_CAPACITY_REMAINING,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_VOLTAGE_PROTECTION_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_VOLTAGE_PROTECTION_BITMASK,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CURRENT_PROTECTION_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_CURRENT_PROTECTION_BITMASK,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_TEMPERATURE_PROTECTION_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_VOLTAGE_PROTECTION_BITMASK,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_ERROR_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_ERROR_BITMASK,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_STATE_OF_CHARGE: {
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_BATTERY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_NOMINAL_CAPACITY: {
        "unit_of_measurement": UNIT_AMPERE_HOURS,
        "icon": ICON_NOMINAL_CAPACITY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_FULL_CHARGE_CAPACITY: {
        "unit_of_measurement": UNIT_AMPERE_HOURS,
        "icon": ICON_FULL_CHARGE_CAPACITY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CHARGING_CYCLES: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_CHARGING_CYCLES,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_MIN_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_MIN_CELL_VOLTAGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MAX_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_MAX_CELL_VOLTAGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MIN_VOLTAGE_CELL: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_MIN_VOLTAGE_CELL,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MAX_VOLTAGE_CELL: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_MAX_VOLTAGE_CELL,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DELTA_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AVERAGE_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 4,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AVERAGE_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AMBIENT_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MOSFET_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_STATE_OF_HEALTH: {
        "unit_of_measurement": UNIT_PERCENT,
        "icon": ICON_STATE_OF_HEALTH,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BALANCED_CELL: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_BALANCED_CELL,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BALANCED_CELL_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_BALANCED_CELL_BITMASK,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_SHORT_CIRCUIT_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CHARGE_OVERCURRENT_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGE_OVERCURRENT_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CELL_OVERVOLTAGE_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CHARGE_UNDERCURRENT_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_PACK_OVERVOLTAGE_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_PACK_UNDERVOLTAGE_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CHARGE_OVERTEMPERATURE_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CHARGE_UNDERTEMPERATURE_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION_COUNT: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_PROTECTION_COUNT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
}

_CELL_VOLTAGE_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_VOLT,
    icon=ICON_EMPTY,
    accuracy_decimals=3,
    device_class=DEVICE_CLASS_VOLTAGE,
    state_class=STATE_CLASS_MEASUREMENT,
)
_TEMPERATURE_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_CELSIUS,
    icon=ICON_EMPTY,
    accuracy_decimals=1,
    device_class=DEVICE_CLASS_TEMPERATURE,
    state_class=STATE_CLASS_MEASUREMENT,
)

CONFIG_SCHEMA = (
    KS_BMS_BLE_COMPONENT_SCHEMA.extend(
        {
            cv.Optional(key): sensor.sensor_schema(**kwargs)
            for key, kwargs in SENSOR_DEFS.items()
        }
    )
    .extend({cv.Optional(key): _CELL_VOLTAGE_SCHEMA for key in CELLS})
    .extend({cv.Optional(key): _TEMPERATURE_SCHEMA for key in TEMPERATURES})
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_KS_BMS_BLE_ID])
    for i, key in enumerate(TEMPERATURES):
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(hub.set_temperature_sensor(i, sens))
    for i, key in enumerate(CELLS):
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(hub.set_cell_voltage_sensor(i, sens))
    for key in SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
