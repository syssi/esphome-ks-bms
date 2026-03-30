#include "ks_switch.h"
#include "../ks_bms_ble.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ks_bms_ble {

static const char *const TAG = "ks_bms_ble.switch";

void KsSwitch::dump_config() { LOG_SWITCH("", "KS BMS Switch", this); }

void KsSwitch::write_state(bool state) {
  if (this->parent_->write_register(this->holding_register_, state ? 0x01 : 0x00)) {
    this->publish_state(state);
  }
}

}  // namespace ks_bms_ble
}  // namespace esphome
