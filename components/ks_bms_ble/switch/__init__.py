import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv

from .. import CONF_KS_BMS_BLE_ID, KS_BMS_BLE_COMPONENT_SCHEMA, ks_bms_ble_ns

DEPENDENCIES = ["ks_bms_ble"]
CODEOWNERS = ["@syssi"]

CONF_CHARGING = "charging"
CONF_DISCHARGING = "discharging"

KsSwitch = ks_bms_ble_ns.class_("KsSwitch", switch.Switch, cg.Component)

SWITCHES = {
    CONF_CHARGING: 0x1D,
    CONF_DISCHARGING: 0x1E,
}

CONFIG_SCHEMA = KS_BMS_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_CHARGING): switch.switch_schema(
            KsSwitch,
            icon="mdi:battery-charging",
        ),
        cv.Optional(CONF_DISCHARGING): switch.switch_schema(
            KsSwitch,
            icon="mdi:power-plug",
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_KS_BMS_BLE_ID])
    for key, address in SWITCHES.items():
        if key in config:
            conf = config[key]
            var = await switch.new_switch(conf)
            await cg.register_component(var, conf)
            cg.add(getattr(hub, f"set_{key}_switch")(var))
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))
