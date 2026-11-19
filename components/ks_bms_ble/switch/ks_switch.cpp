#include "ks_switch.h"
#include "../ks_bms_ble.h"

namespace esphome::ks_bms_ble {

void KsSwitch::write_state(bool state) {
  if (this->parent_->write_register(this->holding_register_, state ? 0x01 : 0x00)) {
    this->publish_state(state);
  }
}

}  // namespace esphome::ks_bms_ble
