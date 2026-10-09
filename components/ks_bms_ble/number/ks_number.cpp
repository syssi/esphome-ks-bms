#include "ks_number.h"
#include "../ks_bms_ble.h"
#include "esphome/core/log.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::ks_bms_ble {

ESPHOME_LOG_TAG(TAG, "ks_bms_ble.number");

void KsNumber::dump_config() { LOG_NUMBER("", "KS BMS Number", this); }

void KsNumber::control(float value) {
  auto payload = (uint16_t) lroundf(value * this->factor_ + this->offset_);
  if (this->parent_->write_register(this->holding_register_, payload)) {
    this->publish_state(value);
  }
}

}  // namespace esphome::ks_bms_ble
