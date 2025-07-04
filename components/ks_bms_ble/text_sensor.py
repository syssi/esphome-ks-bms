import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import CONF_KS_BMS_BLE_ID, KsBmsBle

DEPENDENCIES = ["ks_bms_ble"]

CODEOWNERS = ["@syssi"]

CONF_SOFTWARE_VERSION = "software_version"
CONF_DEVICE_MODEL = "device_model"
CONF_VOLTAGE_PROTECTION = "voltage_protection"
CONF_CURRENT_PROTECTION = "current_protection"
CONF_TEMPERATURE_PROTECTION = "temperature_protection"
CONF_ERRORS = "errors"
CONF_BALANCER_STATUS = "balancer_status"

ICON_DEVICE_MODEL = "mdi:chip"
ICON_SOFTWARE_VERSION = "mdi:numeric"
ICON_VOLTAGE_PROTECTION = "mdi:alert-circle-outline"
ICON_CURRENT_PROTECTION = "mdi:alert-circle-outline"
ICON_TEMPERATURE_PROTECTION = "mdi:alert-circle-outline"
ICON_ERRORS = "mdi:alert-circle-outline"
ICON_BALANCER_STATUS = "mdi:battery-sync"

TEXT_SENSORS = [
    CONF_SOFTWARE_VERSION,
    CONF_DEVICE_MODEL,
    CONF_VOLTAGE_PROTECTION,
    CONF_CURRENT_PROTECTION,
    CONF_TEMPERATURE_PROTECTION,
    CONF_ERRORS,
    CONF_BALANCER_STATUS,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_KS_BMS_BLE_ID): cv.use_id(KsBmsBle),
        cv.Optional(CONF_SOFTWARE_VERSION): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_SOFTWARE_VERSION
        ),
        cv.Optional(CONF_DEVICE_MODEL): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_DEVICE_MODEL
        ),
        cv.Optional(CONF_VOLTAGE_PROTECTION): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_VOLTAGE_PROTECTION
        ),
        cv.Optional(CONF_CURRENT_PROTECTION): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_CURRENT_PROTECTION
        ),
        cv.Optional(CONF_TEMPERATURE_PROTECTION): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_TEMPERATURE_PROTECTION
        ),
        cv.Optional(CONF_ERRORS): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_ERRORS
        ),
        cv.Optional(CONF_BALANCER_STATUS): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_BALANCER_STATUS
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_KS_BMS_BLE_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await text_sensor.register_text_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))
