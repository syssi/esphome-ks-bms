import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import CONF_KS_BMS_BLE_ID, KS_BMS_BLE_COMPONENT_SCHEMA

DEPENDENCIES = ["ks_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_CHARGING = "charging"
CONF_DISCHARGING = "discharging"
CONF_LIMITING_CURRENT = "limiting_current"

# key: binary_sensor_schema kwargs
BINARY_SENSOR_DEFS = {
    CONF_CHARGING: {
        "icon": "mdi:battery-charging",
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGING: {
        "icon": "mdi:power-plug",
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_LIMITING_CURRENT: {
        "icon": "mdi:car-speed-limiter",
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
}

CONFIG_SCHEMA = KS_BMS_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): binary_sensor.binary_sensor_schema(**kwargs)
        for key, kwargs in BINARY_SENSOR_DEFS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_KS_BMS_BLE_ID])
    for key in BINARY_SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
