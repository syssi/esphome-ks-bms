#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

namespace esphome {
namespace ks_bms_ble {

class KsBmsBle;

class KsSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(KsBmsBle *parent) { parent_ = parent; }
  void set_holding_register(uint8_t holding_register) { holding_register_ = holding_register; }
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void write_state(bool state) override;
  KsBmsBle *parent_;
  uint8_t holding_register_;
};

}  // namespace ks_bms_ble
}  // namespace esphome
