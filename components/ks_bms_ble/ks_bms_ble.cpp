#include "ks_bms_ble.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ks_bms_ble {

static const char *const TAG = "ks_bms_ble";

static const uint16_t KS_BMS_SERVICE_UUID = 0xFF00;
static const uint16_t KS_BMS_NOTIFY_CHARACTERISTIC_UUID = 0xFF01;
static const uint16_t KS_BMS_CONTROL_CHARACTERISTIC_UUID = 0xFF02;

static const uint16_t MAX_RESPONSE_SIZE = 20;

static const uint8_t KS_PKT_START = 0x7B;
static const uint8_t KS_FUNCTION_REQUEST = 0x04;
static const uint8_t KS_FUNCTION_RESPONSE = 0x14;
static const uint8_t KS_PKT_END = 0x7D;

static const uint8_t KS_FRAME_TYPE_STATUS = 0x01;

static const uint8_t KS_COMMAND_QUEUE_SIZE = 1;
static const uint8_t KS_COMMAND_QUEUE[KS_COMMAND_QUEUE_SIZE] = {
    KS_FRAME_TYPE_STATUS,
};

static const uint8_t VOLTAGE_PROTECTION_ERRORS_SIZE = 16;
static const char *const VOLTAGE_PROTECTION_ERRORS[VOLTAGE_PROTECTION_ERRORS_SIZE] = {
    "Cell overvoltage protection",         // 0000 0000 0000 0001
    "Cell undervoltage protection",        // 0000 0000 0000 0010
    "Pack overvoltage protection",         // 0000 0000 0000 0100
    "Pack undervoltage protection",        // 0000 0000 0000 1000
    "Cell overvoltage alarm",              // 0000 0000 0001 0000
    "Cell undervoltage alarm",             // 0000 0000 0010 0000
    "Pack overvoltage alarm",              // 0000 0000 0100 0000
    "Pack undervoltage alarm",             // 0000 0000 1000 0000
    "Cell voltage difference alarm",       // 0000 0001 0000 0000
    "Reserved",                            // 0000 0010 0000 0000
    "Reserved",                            // 0000 0100 0000 0000
    "Reserved",                            // 0000 1000 0000 0000
    "Reserved",                            // 0001 0000 0000 0000
    "Reserved",                            // 0010 0000 0000 0000
    "The system is going to sleep state",  // 0100 0000 0000 0000
    "Reserved",                            // 1000 0000 0000 0000
};

static const uint8_t TEMPERATURE_PROTECTION_ERRORS_SIZE = 16;
static const char *const TEMPERATURE_PROTECTION_ERRORS[TEMPERATURE_PROTECTION_ERRORS_SIZE] = {
    "Charge over temperature protection",      // 0000 0000 0000 0001
    "Charge under temperature protection",     // 0000 0000 0000 0010
    "Discharge over temperature protection",   // 0000 0000 0000 0100
    "Discharge under temperature protection",  // 0000 0000 0000 1000
    "Ambient over temperature protection",     // 0000 0000 0001 0000
    "Ambient under temperature protection",    // 0000 0000 0010 0000
    "MOS over temperature protection",         // 0000 0000 0100 0000
    "MOS under temperature protection",        // 0000 0000 1000 0000
    "Charge over temperature alarm",           // 0000 0001 0000 0000
    "Charge under temperature alarm",          // 0000 0010 0000 0000
    "Discharge over temperature alarm",        // 0000 0100 0000 0000
    "Discharge under temperature alarm",       // 0000 1000 0000 0000
    "Ambient over temperature alarm",          // 0001 0000 0000 0000
    "Ambient under temperature alarm",         // 0010 0000 0000 0000
    "MOS over temperature alarm",              // 0100 0000 0000 0000
    "MOS under temperature alarm",             // 1000 0000 0000 0000
};

static const uint8_t CURRENT_PROTECTION_ERRORS_SIZE = 16;
static const char *const CURRENT_PROTECTION_ERRORS[CURRENT_PROTECTION_ERRORS_SIZE] = {
    "Charge overcurrent protection",       // 0000 0000 0000 0001
    "Short circuit protection",            // 0000 0000 0000 0010
    "Discharge overcurrent 1 protection",  // 0000 0000 0000 0100
    "Discharge overcurrent 2 protection",  // 0000 0000 0000 1000
    "Charge overcurrent alarm",            // 0000 0000 0001 0000
    "Discharge overcurrent alarm",         // 0000 0000 0010 0000
    "Gyro lock alarm",                     // 0000 0000 0100 0000
    "Reserved",                            // 0000 0000 1000 0000
    "Reserved",                            // 0000 0001 0000 0000
    "Reserved",                            // 0000 0010 0000 0000
    "Reserved",                            // 0000 0100 0000 0000
    "Reserved",                            // 0000 1000 0000 0000
    "Reserved",                            // 0001 0000 0000 0000
    "Reserved",                            // 0010 0000 0000 0000
    "Reserved",                            // 0100 0000 0000 0000
    "Reserved",                            // 1000 0000 0000 0000
};

static const uint8_t ERRORS_SIZE = 16;
static const char *const ERRORS[ERRORS_SIZE] = {
    "Cell voltage differential alarm",  // 0000 0000 0000 0001
    "Charge MOS damage alarm",          // 0000 0000 0000 0010
    "External SD card failure alarm",   // 0000 0000 0000 0100
    "SPI communication failure alarm",  // 0000 0000 0000 1000
    "EEPROM failure alarm",             // 0000 0000 0001 0000
    "LED alarm enable",                 // 0000 0000 0010 0000
    "Buzzer alarm enable",              // 0000 0000 0100 0000
    "Low battery alarm",                // 0000 0000 1000 0000
    "MOS over temperature protection",  // 0000 0001 0000 0000
    "MOS over temperature alarm",       // 0000 0010 0000 0000
    "Current limiting board failure",   // 0000 0100 0000 0000
    "Sampling failure",                 // 0000 1000 0000 0000
    "Battery failure",                  // 0001 0000 0000 0000
    "NTC failure",                      // 0010 0000 0000 0000
    "Charge MOS failure",               // 0100 0000 0000 0000
    "Discharge MOS failure",            // 1000 0000 0000 0000
};

void KsBmsBle::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                   esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      this->node_state = espbt::ClientState::IDLE;

      // this->publish_state_(this->voltage_sensor_, NAN);

      if (this->char_notify_handle_ != 0) {
        auto status = esp_ble_gattc_unregister_for_notify(this->parent()->get_gattc_if(),
                                                          this->parent()->get_remote_bda(), this->char_notify_handle_);
        if (status) {
          ESP_LOGW(TAG, "esp_ble_gattc_unregister_for_notify failed, status=%d", status);
        }
      }
      this->char_notify_handle_ = 0;
      this->char_command_handle_ = 0;

      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      // [V][esp32_ble_client:192]: [0] [90:A6:BF:93:A0:69] Service UUID: 0x1800
      // [V][esp32_ble_client:194]: [0] [90:A6:BF:93:A0:69]  start_handle: 0x1  end_handle: 0x7
      // [V][esp32_ble_client:192]: [0] [90:A6:BF:93:A0:69] Service UUID: 0x1801
      // [V][esp32_ble_client:194]: [0] [90:A6:BF:93:A0:69]  start_handle: 0x8  end_handle: 0xf
      // [V][esp32_ble_client:192]: [0] [90:A6:BF:93:A0:69] Service UUID: 0xFF00
      // [V][esp32_ble_client:194]: [0] [90:A6:BF:93:A0:69]  start_handle: 0x10  end_handle: 0x18

      auto *char_notify = this->parent_->get_characteristic(KS_BMS_SERVICE_UUID, KS_BMS_NOTIFY_CHARACTERISTIC_UUID);
      if (char_notify == nullptr) {
        ESP_LOGE(TAG, "[%s] No notify service found at device, not an Ks BMS..?", this->parent_->address_str().c_str());
        break;
      }
      this->char_notify_handle_ = char_notify->handle;

      auto status = esp_ble_gattc_register_for_notify(this->parent()->get_gattc_if(), this->parent()->get_remote_bda(),
                                                      char_notify->handle);
      if (status) {
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
      }

      auto *char_command = this->parent_->get_characteristic(KS_BMS_SERVICE_UUID, KS_BMS_CONTROL_CHARACTERISTIC_UUID);
      if (char_command == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not an BASEN BMS..?",
                 this->parent_->address_str().c_str());
        break;
      }
      this->char_command_handle_ = char_command->handle;
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = espbt::ClientState::ESTABLISHED;

      this->send_command_(0x01);
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      ESP_LOGV(TAG, "Notification received (handle 0x%02X): %s", param->notify.handle,
               format_hex_pretty(param->notify.value, param->notify.value_len).c_str());

      std::vector<uint8_t> data(param->notify.value, param->notify.value + param->notify.value_len);

      this->on_ks_bms_ble_data(param->notify.handle, data);
      break;
    }
    default:
      break;
  }
}

void KsBmsBle::update() {
  if (this->node_state != espbt::ClientState::ESTABLISHED) {
    ESP_LOGW(TAG, "[%s] Not connected", this->parent_->address_str().c_str());
    return;
  }

  for (uint8_t command : KS_COMMAND_QUEUE) {
    this->send_command_(command);
  }
}

void KsBmsBle::on_ks_bms_ble_data(const uint8_t &handle, const std::vector<uint8_t> &data) {
  if (data[0] != KS_PKT_START || data.back() != KS_PKT_END || data.size() != MAX_RESPONSE_SIZE) {
    ESP_LOGW(TAG, "Invalid response received: %s", format_hex_pretty(&data.front(), data.size()).c_str());
    return;
  }

  uint8_t frame_type = data[2];

  switch (frame_type) {
    default:
      ESP_LOGW(TAG, "Unhandled response received (frame_type 0x%02X): %s", frame_type,
               format_hex_pretty(&data.front(), data.size()).c_str());
  }
}

void KsBmsBle::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "KsBmsBle:");

  LOG_BINARY_SENSOR("", "Charging", this->charging_binary_sensor_);
  LOG_BINARY_SENSOR("", "Discharging", this->discharging_binary_sensor_);
  LOG_BINARY_SENSOR("", "Limiting current", this->limiting_current_binary_sensor_);

  LOG_SENSOR("", "Total voltage", this->total_voltage_sensor_);
  LOG_SENSOR("", "Current", this->current_sensor_);
  LOG_SENSOR("", "Power", this->power_sensor_);
  LOG_SENSOR("", "Charging power", this->charging_power_sensor_);
  LOG_SENSOR("", "Discharging power", this->discharging_power_sensor_);
  LOG_SENSOR("", "Capacity remaining", this->capacity_remaining_sensor_);
  LOG_SENSOR("", "Voltage protection bitmask", this->voltage_protection_bitmask_sensor_);
  LOG_SENSOR("", "Current protection bitmask", this->current_protection_bitmask_sensor_);
  LOG_SENSOR("", "Temperature protection bitmask", this->temperature_protection_bitmask_sensor_);
  LOG_SENSOR("", "Error bitmask", this->error_bitmask_sensor_);
  LOG_SENSOR("", "State of charge", this->state_of_charge_sensor_);
  LOG_SENSOR("", "Nominal capacity", this->nominal_capacity_sensor_);
  LOG_SENSOR("", "Charging cycles", this->charging_cycles_sensor_);
  LOG_SENSOR("", "Min cell voltage", this->min_cell_voltage_sensor_);
  LOG_SENSOR("", "Max cell voltage", this->max_cell_voltage_sensor_);
  LOG_SENSOR("", "Min voltage cell", this->min_voltage_cell_sensor_);
  LOG_SENSOR("", "Max voltage cell", this->max_voltage_cell_sensor_);
  LOG_SENSOR("", "Delta cell voltage", this->delta_cell_voltage_sensor_);
  LOG_SENSOR("", "Average temperature", this->average_temperature_sensor_);
  LOG_SENSOR("", "Ambient temperature", this->ambient_temperature_sensor_);
  LOG_SENSOR("", "Mosfet temperature", this->mosfet_temperature_sensor_);
  LOG_SENSOR("", "State of health", this->state_of_charge_sensor_);
  LOG_SENSOR("", "Temperature 1", this->temperatures_[0].temperature_sensor_);
  LOG_SENSOR("", "Temperature 2", this->temperatures_[1].temperature_sensor_);
  LOG_SENSOR("", "Temperature 3", this->temperatures_[2].temperature_sensor_);
  LOG_SENSOR("", "Temperature 4", this->temperatures_[3].temperature_sensor_);
  LOG_SENSOR("", "Temperature 5", this->temperatures_[4].temperature_sensor_);
  LOG_SENSOR("", "Temperature 6", this->temperatures_[5].temperature_sensor_);
  LOG_SENSOR("", "Temperature 7", this->temperatures_[6].temperature_sensor_);
  LOG_SENSOR("", "Temperature 8", this->temperatures_[7].temperature_sensor_);
  LOG_SENSOR("", "Cell Voltage 1", this->cells_[0].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 2", this->cells_[1].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 3", this->cells_[2].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 4", this->cells_[3].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 5", this->cells_[4].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 6", this->cells_[5].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 7", this->cells_[6].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 8", this->cells_[7].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 9", this->cells_[8].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 10", this->cells_[9].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 11", this->cells_[10].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 12", this->cells_[11].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 13", this->cells_[12].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 14", this->cells_[13].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 15", this->cells_[14].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 16", this->cells_[15].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 17", this->cells_[16].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 18", this->cells_[17].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 19", this->cells_[18].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 20", this->cells_[19].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 21", this->cells_[20].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 22", this->cells_[21].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 23", this->cells_[22].cell_voltage_sensor_);
  LOG_SENSOR("", "Cell Voltage 24", this->cells_[23].cell_voltage_sensor_);

  LOG_TEXT_SENSOR("", "Voltage protection", this->voltage_protection_text_sensor_);
  LOG_TEXT_SENSOR("", "Current protection", this->current_protection_text_sensor_);
  LOG_TEXT_SENSOR("", "Temperature protection", this->temperature_protection_text_sensor_);
  LOG_TEXT_SENSOR("", "Errors", this->errors_text_sensor_);
}

void KsBmsBle::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void KsBmsBle::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void KsBmsBle::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

void KsBmsBle::write_register(uint8_t address, uint16_t value) {
  // this->send_command_(KS_CMD_WRITE, KS_CMD_MOS);  // @TODO: Pass value
}

bool KsBmsBle::send_command_(uint8_t function) {
  uint8_t frame[4];

  frame[0] = KS_PKT_START;
  frame[1] = function;
  frame[2] = 0x00;
  frame[3] = KS_PKT_END;

  ESP_LOGD(TAG, "Send command (handle 0x%02X): %s", this->char_command_handle_,
           format_hex_pretty(frame, sizeof(frame)).c_str());

  auto status =
      esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), this->char_command_handle_,
                               sizeof(frame), frame, ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);

  if (status) {
    ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(), status);
  }

  return (status == 0);
}

std::string KsBmsBle::bitmask_to_string_(const char *const messages[], const uint8_t &messages_size,
                                         const uint16_t &mask) {
  std::string values = "";
  if (mask) {
    for (int i = 0; i < messages_size; i++) {
      if (mask & (1 << i)) {
        values.append(messages[i]);
        values.append(";");
      }
    }
    if (!values.empty()) {
      values.pop_back();
    }
  }
  return values;
}

}  // namespace ks_bms_ble
}  // namespace esphome
