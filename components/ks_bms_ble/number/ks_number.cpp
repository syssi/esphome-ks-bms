#include "ks_number.h"
#include "../ks_bms_ble.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ks_bms_ble {

static const char *const TAG = "ks_bms_ble.number";

void KsNumber::dump_config() { LOG_NUMBER("", "KS BMS Number", this); }

void KsNumber::control(float value) {
  auto payload = (uint16_t) lroundf(value * this->factor_ + this->offset_);
  if (this->parent_->write_register(this->holding_register_, payload)) {
    this->publish_state(value);
  }
}

}  // namespace ks_bms_ble
}  // namespace esphome
