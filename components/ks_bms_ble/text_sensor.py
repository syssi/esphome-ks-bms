import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import CONF_KS_BMS_BLE_ID, KS_BMS_BLE_COMPONENT_SCHEMA

DEPENDENCIES = ["ks_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_SOFTWARE_VERSION = "software_version"
CONF_HARDWARE_VERSION = "hardware_version"
CONF_BOOTLOADER_VERSION = "bootloader_version"
CONF_SERIAL_NUMBER = "serial_number"
CONF_MANUFACTURING_DATE = "manufacturing_date"
CONF_MODEL_TYPE = "model_type"
CONF_BLUETOOTH_SOFTWARE_VERSION = "bluetooth_software_version"
CONF_DEVICE_MODEL = "device_model"
CONF_VOLTAGE_PROTECTION = "voltage_protection"
CONF_CURRENT_PROTECTION = "current_protection"
CONF_TEMPERATURE_PROTECTION = "temperature_protection"
CONF_ERRORS = "errors"
CONF_BALANCER_STATUS = "balancer_status"

ICON_DEVICE_MODEL = "mdi:chip"
ICON_SOFTWARE_VERSION = "mdi:numeric"
ICON_HARDWARE_VERSION = "mdi:numeric"
ICON_BOOTLOADER_VERSION = "mdi:numeric"
ICON_SERIAL_NUMBER = "mdi:identifier"
ICON_MANUFACTURING_DATE = "mdi:calendar"
ICON_MODEL_TYPE = "mdi:chip"
ICON_BLUETOOTH_SOFTWARE_VERSION = "mdi:bluetooth"
ICON_VOLTAGE_PROTECTION = "mdi:alert-circle-outline"
ICON_CURRENT_PROTECTION = "mdi:alert-circle-outline"
ICON_TEMPERATURE_PROTECTION = "mdi:alert-circle-outline"
ICON_ERRORS = "mdi:alert-circle-outline"
ICON_BALANCER_STATUS = "mdi:battery-sync"

TEXT_SENSORS = [
    CONF_SOFTWARE_VERSION,
    CONF_HARDWARE_VERSION,
    CONF_BOOTLOADER_VERSION,
    CONF_SERIAL_NUMBER,
    CONF_MANUFACTURING_DATE,
    CONF_MODEL_TYPE,
    CONF_BLUETOOTH_SOFTWARE_VERSION,
    CONF_DEVICE_MODEL,
    CONF_VOLTAGE_PROTECTION,
    CONF_CURRENT_PROTECTION,
    CONF_TEMPERATURE_PROTECTION,
    CONF_ERRORS,
    CONF_BALANCER_STATUS,
]

CONFIG_SCHEMA = KS_BMS_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_SOFTWARE_VERSION): text_sensor.text_sensor_schema(
            icon=ICON_SOFTWARE_VERSION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_HARDWARE_VERSION): text_sensor.text_sensor_schema(
            icon=ICON_HARDWARE_VERSION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_BOOTLOADER_VERSION): text_sensor.text_sensor_schema(
            icon=ICON_BOOTLOADER_VERSION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_SERIAL_NUMBER): text_sensor.text_sensor_schema(
            icon=ICON_SERIAL_NUMBER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_MANUFACTURING_DATE): text_sensor.text_sensor_schema(
            icon=ICON_MANUFACTURING_DATE,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_MODEL_TYPE): text_sensor.text_sensor_schema(
            icon=ICON_MODEL_TYPE,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_BLUETOOTH_SOFTWARE_VERSION): text_sensor.text_sensor_schema(
            icon=ICON_BLUETOOTH_SOFTWARE_VERSION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_DEVICE_MODEL): text_sensor.text_sensor_schema(
            icon=ICON_DEVICE_MODEL,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_VOLTAGE_PROTECTION): text_sensor.text_sensor_schema(
            icon=ICON_VOLTAGE_PROTECTION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_CURRENT_PROTECTION): text_sensor.text_sensor_schema(
            icon=ICON_CURRENT_PROTECTION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_TEMPERATURE_PROTECTION): text_sensor.text_sensor_schema(
            icon=ICON_TEMPERATURE_PROTECTION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_ERRORS): text_sensor.text_sensor_schema(
            icon=ICON_ERRORS,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_BALANCER_STATUS): text_sensor.text_sensor_schema(
            icon=ICON_BALANCER_STATUS
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_KS_BMS_BLE_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))
