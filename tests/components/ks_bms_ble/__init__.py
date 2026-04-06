import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

from tests.testing_helpers import ComponentManifestOverride

CONF_DEVICE_TYPE = "device_type"

ks_bms_ble_ns = cg.esphome_ns.namespace("ks_bms_ble")
KsBmsBle = ks_bms_ble_ns.class_("KsBmsBle", cg.PollingComponent)


def override_manifest(manifest: ComponentManifestOverride) -> None:
    # Remove ble_client dependency — not available on host platform.
    manifest.dependencies = []

    manifest.config_schema = cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(KsBmsBle),
            cv.Optional(CONF_DEVICE_TYPE, default=1): cv.int_range(min=1, max=2),
        }
    ).extend(cv.polling_component_schema("20s"))

    async def to_code_stub(config):
        var = cg.new_Pvariable(config[CONF_ID])
        await cg.register_component(var, config)
        cg.add(var.set_device_type(config[CONF_DEVICE_TYPE]))

    manifest.to_code = to_code_stub
