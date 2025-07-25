#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace ks_bms_ble {

namespace espbt = esphome::esp32_ble_tracker;

class KsBmsBle : public esphome::ble_client::BLEClientNode, public PollingComponent {
 public:
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_charging_binary_sensor(binary_sensor::BinarySensor *charging_binary_sensor) {
    charging_binary_sensor_ = charging_binary_sensor;
  }
  void set_discharging_binary_sensor(binary_sensor::BinarySensor *discharging_binary_sensor) {
    discharging_binary_sensor_ = discharging_binary_sensor;
  }
  void set_limiting_current_binary_sensor(binary_sensor::BinarySensor *limiting_current_binary_sensor) {
    limiting_current_binary_sensor_ = limiting_current_binary_sensor;
  }

  void set_total_voltage_sensor(sensor::Sensor *total_voltage_sensor) { total_voltage_sensor_ = total_voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_charging_power_sensor(sensor::Sensor *charging_power_sensor) {
    charging_power_sensor_ = charging_power_sensor;
  }
  void set_discharging_power_sensor(sensor::Sensor *discharging_power_sensor) {
    discharging_power_sensor_ = discharging_power_sensor;
  }
  void set_capacity_remaining_sensor(sensor::Sensor *capacity_remaining_sensor) {
    capacity_remaining_sensor_ = capacity_remaining_sensor;
  }

  void set_voltage_protection_bitmask_sensor(sensor::Sensor *voltage_protection_bitmask_sensor) {
    voltage_protection_bitmask_sensor_ = voltage_protection_bitmask_sensor;
  }
  void set_current_protection_bitmask_sensor(sensor::Sensor *current_protection_bitmask_sensor) {
    current_protection_bitmask_sensor_ = current_protection_bitmask_sensor;
  }
  void set_temperature_protection_bitmask_sensor(sensor::Sensor *temperature_protection_bitmask_sensor) {
    temperature_protection_bitmask_sensor_ = temperature_protection_bitmask_sensor;
  }
  void set_error_bitmask_sensor(sensor::Sensor *error_bitmask_sensor) { error_bitmask_sensor_ = error_bitmask_sensor; }
  void set_state_of_charge_sensor(sensor::Sensor *state_of_charge_sensor) {
    state_of_charge_sensor_ = state_of_charge_sensor;
  }
  void set_nominal_capacity_sensor(sensor::Sensor *nominal_capacity_sensor) {
    nominal_capacity_sensor_ = nominal_capacity_sensor;
  }
  void set_full_charge_capacity_sensor(sensor::Sensor *full_charge_capacity_sensor) {
    full_charge_capacity_sensor_ = full_charge_capacity_sensor;
  }
  void set_charging_cycles_sensor(sensor::Sensor *charging_cycles_sensor) {
    charging_cycles_sensor_ = charging_cycles_sensor;
  }
  void set_average_temperature_sensor(sensor::Sensor *average_temperature_sensor) {
    average_temperature_sensor_ = average_temperature_sensor;
  }
  void set_ambient_temperature_sensor(sensor::Sensor *ambient_temperature_sensor) {
    ambient_temperature_sensor_ = ambient_temperature_sensor;
  }
  void set_mosfet_temperature_sensor(sensor::Sensor *mosfet_temperature_sensor) {
    mosfet_temperature_sensor_ = mosfet_temperature_sensor;
  }
  void set_state_of_health_sensor(sensor::Sensor *state_of_health_sensor) {
    state_of_health_sensor_ = state_of_health_sensor;
  }
  void set_balanced_cell_sensor(sensor::Sensor *balanced_cell_sensor) { balanced_cell_sensor_ = balanced_cell_sensor; }
  void set_balanced_cell_bitmask_sensor(sensor::Sensor *balanced_cell_bitmask_sensor) {
    balanced_cell_bitmask_sensor_ = balanced_cell_bitmask_sensor;
  }

  void set_min_cell_voltage_sensor(sensor::Sensor *min_cell_voltage_sensor) {
    min_cell_voltage_sensor_ = min_cell_voltage_sensor;
  }
  void set_max_cell_voltage_sensor(sensor::Sensor *max_cell_voltage_sensor) {
    max_cell_voltage_sensor_ = max_cell_voltage_sensor;
  }
  void set_min_voltage_cell_sensor(sensor::Sensor *min_voltage_cell_sensor) {
    min_voltage_cell_sensor_ = min_voltage_cell_sensor;
  }
  void set_max_voltage_cell_sensor(sensor::Sensor *max_voltage_cell_sensor) {
    max_voltage_cell_sensor_ = max_voltage_cell_sensor;
  }
  void set_delta_cell_voltage_sensor(sensor::Sensor *delta_cell_voltage_sensor) {
    delta_cell_voltage_sensor_ = delta_cell_voltage_sensor;
  }
  void set_average_cell_voltage_sensor(sensor::Sensor *average_cell_voltage_sensor) {
    average_cell_voltage_sensor_ = average_cell_voltage_sensor;
  }
  void set_cell_voltage_sensor(uint8_t cell, sensor::Sensor *cell_voltage_sensor) {
    this->cells_[cell].cell_voltage_sensor_ = cell_voltage_sensor;
  }
  void set_temperature_sensor(uint8_t temperature, sensor::Sensor *temperature_sensor) {
    this->temperatures_[temperature].temperature_sensor_ = temperature_sensor;
  }

  void set_software_version_text_sensor(text_sensor::TextSensor *software_version_text_sensor) {
    software_version_text_sensor_ = software_version_text_sensor;
  }
  void set_device_model_text_sensor(text_sensor::TextSensor *device_model_text_sensor) {
    device_model_text_sensor_ = device_model_text_sensor;
  }
  void set_voltage_protection_text_sensor(text_sensor::TextSensor *voltage_protection_text_sensor) {
    voltage_protection_text_sensor_ = voltage_protection_text_sensor;
  }
  void set_current_protection_text_sensor(text_sensor::TextSensor *current_protection_text_sensor) {
    current_protection_text_sensor_ = current_protection_text_sensor;
  }
  void set_temperature_protection_text_sensor(text_sensor::TextSensor *temperature_protection_text_sensor) {
    temperature_protection_text_sensor_ = temperature_protection_text_sensor;
  }
  void set_errors_text_sensor(text_sensor::TextSensor *errors_text_sensor) { errors_text_sensor_ = errors_text_sensor; }
  void set_balancer_status_text_sensor(text_sensor::TextSensor *balancer_status_text_sensor) {
    balancer_status_text_sensor_ = balancer_status_text_sensor;
  }

  void write_register(uint8_t address, uint16_t value);
  void on_ks_bms_ble_data(const uint8_t &handle, const std::vector<uint8_t> &data);

 protected:
  binary_sensor::BinarySensor *charging_binary_sensor_;
  binary_sensor::BinarySensor *discharging_binary_sensor_;
  binary_sensor::BinarySensor *limiting_current_binary_sensor_;

  sensor::Sensor *total_voltage_sensor_;
  sensor::Sensor *current_sensor_;
  sensor::Sensor *power_sensor_;
  sensor::Sensor *charging_power_sensor_;
  sensor::Sensor *discharging_power_sensor_;
  sensor::Sensor *capacity_remaining_sensor_;
  sensor::Sensor *voltage_protection_bitmask_sensor_;
  sensor::Sensor *current_protection_bitmask_sensor_;
  sensor::Sensor *temperature_protection_bitmask_sensor_;
  sensor::Sensor *error_bitmask_sensor_;
  sensor::Sensor *state_of_charge_sensor_;
  sensor::Sensor *nominal_capacity_sensor_;
  sensor::Sensor *full_charge_capacity_sensor_;
  sensor::Sensor *charging_cycles_sensor_;
  sensor::Sensor *min_cell_voltage_sensor_;
  sensor::Sensor *max_cell_voltage_sensor_;
  sensor::Sensor *min_voltage_cell_sensor_;
  sensor::Sensor *max_voltage_cell_sensor_;
  sensor::Sensor *delta_cell_voltage_sensor_;
  sensor::Sensor *average_cell_voltage_sensor_;
  sensor::Sensor *average_temperature_sensor_;
  sensor::Sensor *ambient_temperature_sensor_;
  sensor::Sensor *mosfet_temperature_sensor_;
  sensor::Sensor *state_of_health_sensor_;
  sensor::Sensor *balanced_cell_sensor_;
  sensor::Sensor *balanced_cell_bitmask_sensor_;

  text_sensor::TextSensor *software_version_text_sensor_;
  text_sensor::TextSensor *device_model_text_sensor_;
  text_sensor::TextSensor *voltage_protection_text_sensor_;
  text_sensor::TextSensor *current_protection_text_sensor_;
  text_sensor::TextSensor *temperature_protection_text_sensor_;
  text_sensor::TextSensor *errors_text_sensor_;
  text_sensor::TextSensor *balancer_status_text_sensor_;

  struct Cell {
    sensor::Sensor *cell_voltage_sensor_{nullptr};
  } cells_[24];

  struct Temperature {
    sensor::Sensor *temperature_sensor_{nullptr};
  } temperatures_[8];

  std::vector<uint8_t> frame_buffer_;
  uint16_t char_notify_handle_;
  uint16_t char_command_handle_;

  void decode_status_data_(const std::vector<uint8_t> &data);
  void decode_cell_voltages_data_(const std::vector<uint8_t> &data);
  void decode_temperatures_data_(const std::vector<uint8_t> &data);
  void decode_manufacturing_date_data_(const std::vector<uint8_t> &data);
  void decode_model_name_data_(const std::vector<uint8_t> &data);
  void decode_serial_number_data_(const std::vector<uint8_t> &data);
  void decode_model_type_data_(const std::vector<uint8_t> &data);
  void decode_bluetooth_software_version_data_(const std::vector<uint8_t> &data);
  void decode_software_version_data_(const std::vector<uint8_t> &data);
  void decode_hardware_version_data_(const std::vector<uint8_t> &data);
  void decode_bootloader_version_data_(const std::vector<uint8_t> &data);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
  bool send_command_(uint8_t function);
  std::string bitmask_to_string_(const char *const messages[], const uint8_t &messages_size, const uint16_t &mask);
  std::string fet_control_status_to_balancer_status_text_(uint16_t fet_control_status);

  bool check_bit_(uint16_t mask, uint16_t flag) { return (mask & flag) == flag; }
};

}  // namespace ks_bms_ble
}  // namespace esphome

#endif
