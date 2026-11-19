#pragma once

#include "esphome/components/switch/switch.h"

namespace esphome::ks_bms_ble {

class KsBmsBle;

class KsSwitch : public switch_::Switch {
 public:
  void set_parent(KsBmsBle *parent) { parent_ = parent; }
  void set_holding_register(uint8_t holding_register) { holding_register_ = holding_register; }

 protected:
  void write_state(bool state) override;
  KsBmsBle *parent_;
  uint8_t holding_register_;
};

}  // namespace esphome::ks_bms_ble
