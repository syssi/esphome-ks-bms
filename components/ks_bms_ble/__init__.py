import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]
DEPENDENCIES = ["ble_client"]
AUTO_LOAD = ["binary_sensor", "number", "sensor", "switch", "text_sensor"]
MULTI_CONF = True

CONF_KS_BMS_BLE_ID = "ks_bms_ble_id"
CONF_DEVICE_TYPE = "device_type"

ks_bms_ble_ns = cg.esphome_ns.namespace("ks_bms_ble")
KsBmsBle = ks_bms_ble_ns.class_(
    "KsBmsBle", ble_client.BLEClientNode, cg.PollingComponent
)

KS_BMS_BLE_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_KS_BMS_BLE_ID): cv.use_id(KsBmsBle),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.require_esphome_version(2024, 12, 0),
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(KsBmsBle),
            cv.Optional(CONF_DEVICE_TYPE, default=1): cv.int_range(min=1, max=2),
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.polling_component_schema("20s")),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
    cg.add(var.set_device_type(config[CONF_DEVICE_TYPE]))
