#pragma once

#include "esphome/components/number/number.h"
#include "esphome/core/component.h"

namespace esphome {
namespace ks_bms_ble {

class KsBmsBle;

class KsNumber : public number::Number, public Component {
 public:
  void set_parent(KsBmsBle *parent) { this->parent_ = parent; }
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; }
  void set_factor(float factor) { this->factor_ = factor; }
  void set_offset(float offset) { this->offset_ = offset; }
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void control(float value) override;

  KsBmsBle *parent_;
  uint8_t holding_register_;
  float factor_{1.0f};
  float offset_{0.0f};
};

}  // namespace ks_bms_ble
}  // namespace esphome
