#include "ks_switch.h"
#include "../ks_bms_ble.h"
#include "esphome/core/log.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::ks_bms_ble {

ESPHOME_LOG_TAG(TAG, "ks_bms_ble.switch");

void KsSwitch::dump_config() { LOG_SWITCH("", "KS BMS Switch", this); }

void KsSwitch::write_state(bool state) {
  if (this->parent_->write_register(this->holding_register_, state ? 0x01 : 0x00)) {
    this->publish_state(state);
  }
}

}  // namespace esphome::ks_bms_ble
