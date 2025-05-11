#include "ks_bms_ble.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ks_bms_ble {

static const char *const TAG = "ks_bms_ble";

static const uint16_t KS_BMS_SERVICE_UUID = 0xFF00;
static const uint16_t KS_BMS_NOTIFY_CHARACTERISTIC_UUID = 0xFF01;
static const uint16_t KS_BMS_CONTROL_CHARACTERISTIC_UUID = 0xFF02;  // handle 0x10

static const uint16_t MAX_RESPONSE_SIZE = 40;

static const uint8_t KS_PKT_START = 0x7B;
static const uint8_t KS_PKT_END = 0x7D;

static const uint8_t KS_FRAME_TYPE_STATUS = 0x01;
static const uint8_t KS_FRAME_TYPE_CELL_VOLTAGES = 0x02;
static const uint8_t KS_FRAME_TYPE_TEMPERATURES = 0x03;
static const uint8_t KS_FRAME_TYPE_HISTORY = 0x08;
static const uint8_t KS_FRAME_TYPE_MANUFACTURING_DATE = 0x09;
static const uint8_t KS_FRAME_TYPE_MODEL_NAME = 0x0A;
static const uint8_t KS_FRAME_TYPE_SERIAL_NUMBER = 0x0B;
static const uint8_t KS_FRAME_TYPE_MODEL_TYPE = 0x0C;
static const uint8_t KS_FRAME_TYPE_STATUS_BITMASK = 0x64;  // No response
static const uint8_t KS_FRAME_TYPE_BLUETOOTH_SOFTWARE_VERSION = 0x74;
static const uint8_t KS_FRAME_TYPE_SOFTWARE_VERSION = 0xF3;  // No response
static const uint8_t KS_FRAME_TYPE_HARDWARE_VERSION = 0xF4;
static const uint8_t KS_FRAME_TYPE_BOOTLOADER_VERSION = 0xF5;

static const uint8_t KS_COMMAND_QUEUE_SIZE = 12;
static const uint8_t KS_COMMAND_QUEUE[KS_COMMAND_QUEUE_SIZE] = {
    KS_FRAME_TYPE_STATUS,
    KS_FRAME_TYPE_CELL_VOLTAGES,
    KS_FRAME_TYPE_TEMPERATURES,
    KS_FRAME_TYPE_HISTORY,
    KS_FRAME_TYPE_MANUFACTURING_DATE,
    KS_FRAME_TYPE_MODEL_NAME,
    KS_FRAME_TYPE_SERIAL_NUMBER,
    KS_FRAME_TYPE_MODEL_TYPE,
    KS_FRAME_TYPE_HARDWARE_VERSION,
    KS_FRAME_TYPE_BOOTLOADER_VERSION,
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
  if (data[0] != KS_PKT_START || data.back() != KS_PKT_END || (data.size() > 3 && data.size() != data[2] + 4) ||
      data.size() > MAX_RESPONSE_SIZE) {
    ESP_LOGW(TAG, "Invalid response received: %s", format_hex_pretty(&data.front(), data.size()).c_str());
    return;
  }

  uint8_t frame_type = data[1];

  switch (frame_type) {
    case KS_FRAME_TYPE_STATUS:
      this->decode_status_data_(data);
      break;
    case KS_FRAME_TYPE_CELL_VOLTAGES:
      this->decode_cell_voltages_data_(data);
      break;
    case KS_FRAME_TYPE_TEMPERATURES:
      this->decode_temperatures_data_(data);
      break;
    case KS_FRAME_TYPE_MANUFACTURING_DATE:
      this->decode_manufacturing_date_data_(data);
      break;
    case KS_FRAME_TYPE_MODEL_NAME:
      this->decode_model_name_data_(data);
      break;
    case KS_FRAME_TYPE_SERIAL_NUMBER:
      this->decode_serial_number_data_(data);
      break;
    case KS_FRAME_TYPE_MODEL_TYPE:
      this->decode_model_type_data_(data);
      break;
    case KS_FRAME_TYPE_BLUETOOTH_SOFTWARE_VERSION:
      this->decode_bluetooth_software_version_data_(data);
      break;
    case KS_FRAME_TYPE_SOFTWARE_VERSION:
      this->decode_software_version_data_(data);
      break;
    case KS_FRAME_TYPE_HARDWARE_VERSION:
      this->decode_hardware_version_data_(data);
      break;
    case KS_FRAME_TYPE_BOOTLOADER_VERSION:
      this->decode_bootloader_version_data_(data);
      break;

    default:
      ESP_LOGW(TAG, "Unhandled response received (frame_type 0x%02X): %s", frame_type,
               format_hex_pretty(&data.front(), data.size()).c_str());
  }
}

void KsBmsBle::decode_status_data_(const std::vector<uint8_t> &data) {
  auto ks_get_16bit = [&](size_t i) -> uint16_t { return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0); };
  auto ks_get_balancer_status = [&](size_t i) -> uint32_t {
    return (data[i + 0] << 24) | (data[i + 1] << 16) | (data[i + 2] << 8) | data[i + 3];
  };

  ESP_LOGI(TAG, "Status frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x01         Frame type
  //  2    1  0x20         Data length
  //  3    2  0x00 0x45    State of charge (69%)              %   1.0    69%
  this->publish_state_(this->state_of_charge_sensor_, ks_get_16bit(3) * 1.0f);

  //  5    2  0x14 0x8D    Total voltage
  float total_voltage = ks_get_16bit(5) * 0.01f;
  this->publish_state_(this->total_voltage_sensor_, total_voltage);

  //  7    2  0x00 0xDC    Average temperature?
  this->publish_state_(this->average_temperature_sensor_, ((int16_t) ks_get_16bit(7)) * 0.1f);

  //  9    2  0x00 0xB4    Ambient temperature?
  this->publish_state_(this->ambient_temperature_sensor_, ((int16_t) ks_get_16bit(9)) * 0.1f);

  // 11    2  0x00 0xB4    Mosfet temperature
  this->publish_state_(this->mosfet_temperature_sensor_, ((int16_t) ks_get_16bit(11)) * 0.1f);

  // 13    2  0x00 0x00    Current
  float current = ((int16_t) ks_get_16bit(13)) * 0.01f;
  this->publish_state_(this->current_sensor_, current);
  float power = total_voltage * current;
  this->publish_state_(this->power_sensor_, power);
  this->publish_state_(this->charging_power_sensor_, std::max(0.0f, power));               // 500W vs 0W -> 500W
  this->publish_state_(this->discharging_power_sensor_, std::abs(std::min(0.0f, power)));  // -500W vs 0W -> 500W

  // 15    2  0x52 0x05    Remaining capacity
  this->publish_state_(this->capacity_remaining_sensor_, ks_get_16bit(15) * 0.01f);

  // 17    2  0x75 0x30    Full capacity
  ESP_LOGI(TAG, "Full capacity: %.2f Ah", ks_get_16bit(17) * 0.01f);

  // 19    2  0x75 0x30    Nominal capacity
  this->publish_state_(this->nominal_capacity_sensor_, ks_get_16bit(19) * 0.01f);

  // 21    2  0x00 0x00    Unknown
  ESP_LOGD(TAG, "Unknown21: %d (0x%02X 0x%02X)", ks_get_16bit(21), data[21], data[22]);

  // 23    2  0x00 0x01    Number of cycles
  this->publish_state_(this->charging_cycles_sensor_, ks_get_16bit(23) * 1.0f);

  // 25    4  0x00 0x00 0x00 0x00    Balancer status (balanced cell)
  ESP_LOGI(TAG, "Balancer status: %lu", (unsigned long) ks_get_balancer_status(25));

  // 29    2  0x00 0x0C    FET control status
  //                         Bit 0: Balancer charging
  //                         Bit 1: Balancer discharging
  //                         Bit 2: Charging
  //                         Bit 3: Discharging
  ESP_LOGD(TAG, "FET control status: %d (0x%02X 0x%02X)", ks_get_16bit(29), data[29], data[30]);
  this->publish_state_(this->charging_binary_sensor_, (bool) check_bit_(ks_get_16bit(29), 8));
  this->publish_state_(this->discharging_binary_sensor_, (bool) check_bit_(ks_get_16bit(29), 4));
  // @TODO: Add balancing indicator

  // 31    2  0x00 0x00    Protection status
  //                         Bit 0: Over Current Protection
  //                         Bit 1: Under Current Protection
  //                         Bit 2: Over Voltage Protection
  //                         Bit 3: Under Voltage Protection
  //                         Bit 4: Over Temperature Charge Protection
  //                         Bit 5: Under Temperature Charge Protection
  //                         Bit 6: Over Temperature Discharge Protection
  //                         Bit 7: Under Temperature Discharge Protection
  //                         Bit 8: Over Current Charge Protection
  //                         Bit 9: Over Current Discharge Protection
  //                         Bit 10: Short Circuit Protection
  //                         Bit 11: Analog Front-End Error
  //                         Bit 12: Soft Lock MOS
  //                         Bit 13: Charge MOSFET Error
  //                         Bit 14: Discharge MOSFET Error
  //                         Bit 15:Reserved
  ESP_LOGI(TAG, "Protection status: %d", ks_get_16bit(31));

  // 33    2  0x00 0x64    State of Health (if available)
  if (data[2] > 19) {
    this->publish_state_(this->state_of_health_sensor_, ks_get_16bit(33) * 1.0f);
  }

  // 34    1  0x7D         End of frame
}

void KsBmsBle::decode_cell_voltages_data_(const std::vector<uint8_t> &data) {
  auto ks_get_16bit = [&](size_t i) -> uint16_t { return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0); };

  ESP_LOGI(TAG, "Cell voltages frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x02         Frame type
  //  2    1  0x21         Data length
  //  3    1  0x10         Cell count

  //  4    2  0x0C 0xD8    Cell voltage 1
  //  6    2  0x0C 0xDA    Cell voltage 2
  //  8    2  0x0C 0xD9    Cell voltage 3
  // 10    2  0x0C 0xD8    Cell voltage 4
  // 12    2  0x0C 0xD8    Cell voltage 5
  // 14    2  0x0C 0xD8    Cell voltage 6
  // 16    2  0x0C 0xD9    Cell voltage 7
  // 18    2  0x0C 0xD8    Cell voltage 8
  // 20    2  0x0C 0xD8    Cell voltage 9
  // 22    2  0x0C 0xD9    Cell voltage 10
  // 24    2  0x0C 0xD9    Cell voltage 11
  // 26    2  0x0C 0xD8    Cell voltage 12
  // 28    2  0x0C 0xD9    Cell voltage 13
  // 30    2  0x0C 0xD9    Cell voltage 14
  // 32    2  0x0C 0xD8    Cell voltage 15
  // 34    2  0x0C 0xD6    Cell voltage 16

  if (data[3] > 24) {
    ESP_LOGW(TAG, "Found %d cells, but current implementation only supports 24 cells. Please create an issue!",
             data[3]);
  }

  uint8_t cells = std::min(data[3], (uint8_t) 24);
  float min_cell_voltage = 100.0f;
  float max_cell_voltage = -100.0f;
  float average_cell_voltage = 0.0f;
  uint8_t min_voltage_cell = 0;
  uint8_t max_voltage_cell = 0;

  for (uint8_t i = 0; i < cells; i++) {
    float cell_voltage = ks_get_16bit((i * 2) + 4) * 0.001f;
    if (cell_voltage < min_cell_voltage) {
      min_cell_voltage = cell_voltage;
      min_voltage_cell = i + 1;
    }
    if (cell_voltage > max_cell_voltage) {
      max_cell_voltage = cell_voltage;
      max_voltage_cell = i + 1;
    }
    this->publish_state_(this->cells_[i].cell_voltage_sensor_, cell_voltage);
  }

  this->publish_state_(this->min_cell_voltage_sensor_, min_cell_voltage);
  this->publish_state_(this->max_cell_voltage_sensor_, max_cell_voltage);
  this->publish_state_(this->min_voltage_cell_sensor_, (float) min_voltage_cell);
  this->publish_state_(this->max_voltage_cell_sensor_, (float) max_voltage_cell);
  this->publish_state_(this->delta_cell_voltage_sensor_, max_cell_voltage - min_cell_voltage);

  // 36    1  0x7D         End of frame
}

void KsBmsBle::decode_temperatures_data_(const std::vector<uint8_t> &data) {
  auto ks_get_16bit = [&](size_t i) -> uint16_t { return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0); };

  ESP_LOGI(TAG, "Temperatures frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x03         Frame type
  //  2    1  0x09         Data length
  //  3    1  0x04         Temperature sensor count
  //  4    2  0x0B 0x5F    Temperature 1
  //  6    2  0x0B 0x5F    Temperature 2
  //  8    2  0x0B 0x5F    Temperature 3
  // 10    2  0x0B 0x5F    Temperature 4

  if (data[3] > 8) {
    ESP_LOGW(TAG,
             "Found %d temperature probes, but current implementation only supports 8 temperature sensors. Please "
             "create an issue!",
             data[3]);
  }

  uint8_t temperatures = std::min(data[3], (uint8_t) 8);
  for (uint8_t i = 0; i < temperatures; i++) {
    this->publish_state_(this->temperatures_[i].temperature_sensor_, (ks_get_16bit((i * 2) + 4) - 2731) * 0.1f);
  }

  // 12    1  0x7D         End of frame
}

void KsBmsBle::decode_manufacturing_date_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Manufacturing date frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x09         Frame type
  //  2    1  0x03         Data length
  //  3    1  0xFF         Year + 2000
  //  4    1  0xFF         Month
  //  5    1  0xFF         Day
  if (data[3] == 0xff && data[4] == 0xff && data[5] == 0xff) {
    ESP_LOGI(TAG, "Manufacturing date: unset");
  } else {
    ESP_LOGI(TAG, "Manufacturing date: %d.%d.%d", data[3] + 2000, data[4], data[5]);
  }

  //  6    1  0x7D         End of frame
}

void KsBmsBle::decode_model_name_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Model name frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x0A         Frame type
  //  2    1  0x1E         Data length
  //  3   30  0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF         Model name
  //          0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
  //          0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
  if (data[3] == 0xff && data[4] == 0xff && data[31] == 0xff && data[32] == 0xff) {
    ESP_LOGI(TAG, "Model name: unset");
  } else {
    ESP_LOGI(TAG, "Model name: %s", std::string(data.begin() + 3, data.begin() + 3 + 30).c_str());
  }

  //  33    1  0x7D         End of frame
}

void KsBmsBle::decode_serial_number_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Serial number frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x0B         Frame type
  //  2    1  0x13         Data length
  //  3   19  0x43 0x45 0x52 0x32 0x34 0x31 0x30 0x2D 0x30 0x31         Serial number
  //          0x38 0x2D 0x30 0x33 0x30 0x2D 0x30 0x32 0x33
  if (data[3] == 0xff && data[4] == 0xff && data[14] == 0xff && data[15] == 0xff) {
    ESP_LOGI(TAG, "Serial number: unset");
  } else {
    ESP_LOGI(TAG, "Serial number: %s", std::string(data.begin() + 3, data.begin() + 3 + 19).c_str());
  }

  //  16   1  0x7D         End of frame
}

void KsBmsBle::decode_model_type_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Model type frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x0C         Frame type
  //  2    1  0x14
  //  3   20  0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF         Model type
  //          0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
  if (data[3] == 0xff && data[4] == 0xff && data[21] == 0xff && data[22] == 0xff) {
    ESP_LOGI(TAG, "Model type: unset");
  } else {
    ESP_LOGI(TAG, "Model type: %s", std::string(data.begin() + 3, data.begin() + 3 + 20).c_str());
  }

  // 23    1  0x7D         End of frame
}

void KsBmsBle::decode_bluetooth_software_version_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Bluetooth software version frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0x74         Frame type
  //  2    1  0x18         Data length
  //  3   24  0x4B 0x53 0x5F 0x42 0x4C 0x45 0x5F 0x56 0x65 0x72
  //          0x31 0x2E 0x30 0x2E 0x30 0x5F 0x32 0x30 0x32 0x34
  //          0x30 0x37 0x31 0x36
  if (data[3] == 0xff && data[4] == 0xff && data[25] == 0xff && data[26] == 0xff) {
    ESP_LOGI(TAG, "Bluetooth software version: unset");
  } else {
    ESP_LOGI(TAG, "Bluetooth software version: %s", std::string(data.begin() + 3, data.begin() + 3 + 24).c_str());
  }

  // 27    1  0x7D         End of frame
}

void KsBmsBle::decode_software_version_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Software version frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0xF3         Frame type
  //  2    1  0x02         Data length
  //  3    1  0x00         Main version
  //  4    1  0x00         Sub version
  ESP_LOGI(TAG, "Software version: %d.%d", data[3], data[4]);

  //  5    1  0x7D         End of frame
}

void KsBmsBle::decode_hardware_version_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Hardware version frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0xF4         Frame type
  //  2    1  0x03         Data length
  //  3    1  0x02         Main version
  //  4    1  0x02         Sub version
  //  5    1  0x01         Build version
  ESP_LOGI(TAG, "Hardware version: %d.%d.%d", data[3], data[4], data[5]);

  //  6    1  0x7D         End of frame
}

void KsBmsBle::decode_bootloader_version_data_(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "Bootloader version frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  // Byte Len Payload      Description                      Unit  Precision
  //  0    1  0x7B         Start of frame
  //  1    1  0xF5         Frame type
  //  2    1  0x02         Data length
  //  3    1  0x02         Main version
  //  4    1  0x39         Sub version
  ESP_LOGI(TAG, "Bootloader version: %d.%d", data[3], data[4]);

  //  5    1  0x7D         End of frame
}

void KsBmsBle::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "KsBmsBle:");

  LOG_BINARY_SENSOR("", "Charging", this->charging_binary_sensor_);
  LOG_BINARY_SENSOR("", "Discharging", this->discharging_binary_sensor_);
  LOG_BINARY_SENSOR("", "Limiting current", this->limiting_current_binary_sensor_);

  LOG_SENSOR("", "State of charge", this->state_of_charge_sensor_);
  LOG_SENSOR("", "Total voltage", this->total_voltage_sensor_);
  LOG_SENSOR("", "Average temperature", this->average_temperature_sensor_);
  LOG_SENSOR("", "Ambient temperature", this->ambient_temperature_sensor_);
  LOG_SENSOR("", "Mosfet temperature", this->mosfet_temperature_sensor_);
  LOG_SENSOR("", "Current", this->current_sensor_);
  LOG_SENSOR("", "Power", this->power_sensor_);
  LOG_SENSOR("", "Charging power", this->charging_power_sensor_);
  LOG_SENSOR("", "Discharging power", this->discharging_power_sensor_);
  LOG_SENSOR("", "Capacity remaining", this->capacity_remaining_sensor_);

  LOG_SENSOR("", "Nominal capacity", this->nominal_capacity_sensor_);
  LOG_SENSOR("", "Charging cycles", this->charging_cycles_sensor_);

  LOG_SENSOR("", "State of health", this->state_of_charge_sensor_);

  LOG_SENSOR("", "Min cell voltage", this->min_cell_voltage_sensor_);
  LOG_SENSOR("", "Max cell voltage", this->max_cell_voltage_sensor_);
  LOG_SENSOR("", "Min voltage cell", this->min_voltage_cell_sensor_);
  LOG_SENSOR("", "Max voltage cell", this->max_voltage_cell_sensor_);
  LOG_SENSOR("", "Delta cell voltage", this->delta_cell_voltage_sensor_);
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
  LOG_SENSOR("", "Temperature 1", this->temperatures_[0].temperature_sensor_);
  LOG_SENSOR("", "Temperature 2", this->temperatures_[1].temperature_sensor_);
  LOG_SENSOR("", "Temperature 3", this->temperatures_[2].temperature_sensor_);
  LOG_SENSOR("", "Temperature 4", this->temperatures_[3].temperature_sensor_);
  LOG_SENSOR("", "Temperature 5", this->temperatures_[4].temperature_sensor_);
  LOG_SENSOR("", "Temperature 6", this->temperatures_[5].temperature_sensor_);
  LOG_SENSOR("", "Temperature 7", this->temperatures_[6].temperature_sensor_);
  LOG_SENSOR("", "Temperature 8", this->temperatures_[7].temperature_sensor_);
  LOG_SENSOR("", "State of health", this->state_of_health_sensor_);

  LOG_SENSOR("", "Voltage protection bitmask", this->voltage_protection_bitmask_sensor_);
  LOG_SENSOR("", "Current protection bitmask", this->current_protection_bitmask_sensor_);
  LOG_SENSOR("", "Temperature protection bitmask", this->temperature_protection_bitmask_sensor_);
  LOG_SENSOR("", "Error bitmask", this->error_bitmask_sensor_);

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
