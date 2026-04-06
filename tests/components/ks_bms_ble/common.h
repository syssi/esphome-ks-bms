#pragma once
#include <cstdint>
#include <vector>
#include "esphome/components/ks_bms_ble/ks_bms_ble.h"

namespace esphome::ks_bms_ble::testing {

// Exposes protected decoder methods for direct testing.
class TestableKsBmsBle : public KsBmsBle {
 public:
  using KsBmsBle::decode_bluetooth_software_version_data_;
  using KsBmsBle::decode_bootloader_version_data_;
  using KsBmsBle::decode_cell_voltages_data_;
  using KsBmsBle::decode_hardware_version_data_;
  using KsBmsBle::decode_manufacturing_date_data_;
  using KsBmsBle::decode_model_name_data_;
  using KsBmsBle::decode_model_type_data_;
  using KsBmsBle::decode_serial_number_data_;
  using KsBmsBle::decode_software_version_data_;
  using KsBmsBle::decode_status_data_;
  using KsBmsBle::decode_temperatures_data_;
  using KsBmsBle::on_ks_bms_ble_data;
};

// Concrete Switch implementation for testing (write_state is pure virtual).
class TestSwitch : public switch_::Switch {
 protected:
  void write_state(bool state) override { this->publish_state(state); }
};

// ── Real frames from esp32-ble-example-faker.yaml ────────────────────────────
//
// Values are decoded inline next to each relevant byte group so the test
// expectations can be verified by reading the frame definition alone.

// SOC=69%  voltage=52.61V  avg=22.0°C  ambient=18.0°C  mosfet=18.0°C
// current=0A  remaining=209.97Ah  full=300Ah  nominal=300Ah  cycles=1
// balanced=0  FET=0x000C(charging+discharging)  errors=0  SOH=100%
static const std::vector<uint8_t> STATUS_FRAME_1 = {
    0x7B, 0x01, 0x20,        // start | type=STATUS | data_len=32
    0x00, 0x45,              // SOC        = 0x0045 = 69 %
    0x14, 0x8D,              // voltage    = 0x148D = 5261 * 0.01 = 52.61 V
    0x00, 0xDC,              // avg temp   = 0x00DC = 220  * 0.1  = 22.0 °C
    0x00, 0xB4,              // ambient    = 0x00B4 = 180  * 0.1  = 18.0 °C
    0x00, 0xB4,              // mosfet     = 0x00B4 = 180  * 0.1  = 18.0 °C
    0x00, 0x00,              // current    = 0x0000 = 0    * 0.01 = 0.00 A
    0x52, 0x05,              // remaining  = 0x5205 = 20997 * 0.01 = 209.97 Ah
    0x75, 0x30,              // full       = 0x7530 = 30000 * 0.01 = 300.00 Ah
    0x75, 0x30,              // nominal    = 0x7530 = 30000 * 0.01 = 300.00 Ah
    0x00, 0x00,              // unknown
    0x00, 0x01,              // cycles     = 1
    0x00, 0x00, 0x00, 0x00,  // balanced bitmask = 0 (no cell balancing)
    0x00, 0x0C,              // FET        = 0x000C: bit2(4)=charging bit3(8)=discharging
    0x00, 0x00,              // errors     = 0 (no faults)
    0x00, 0x64,              // SOH        = 0x0064 = 100 %
    0x7D,                    // end
};

// SOC=69%  voltage=52.59V  avg=18.0°C  ambient=13.7°C  mosfet=14.0°C
// current=0A  remaining=209.92Ah  full=300Ah  nominal=300Ah  cycles=1
// balanced=0  FET=0x000C(charging+discharging)  errors=0  SOH=100%
static const std::vector<uint8_t> STATUS_FRAME_2 = {
    0x7B, 0x01, 0x20, 0x00, 0x45,  // SOC        = 69 %
    0x14, 0x8B,                    // voltage    = 0x148B = 5259 * 0.01 = 52.59 V
    0x00, 0xB4,                    // avg temp   = 18.0 °C
    0x00, 0x89,                    // ambient    = 0x0089 = 137  * 0.1  = 13.7 °C
    0x00, 0x8C,                    // mosfet     = 0x008C = 140  * 0.1  = 14.0 °C
    0x00, 0x00,                    // current    = 0.00 A
    0x52, 0x00,                    // remaining  = 0x5200 = 20992 * 0.01 = 209.92 Ah
    0x75, 0x30,                    // full       = 300.00 Ah
    0x75, 0x30,                    // nominal    = 300.00 Ah
    0x00, 0x00, 0x00, 0x01,        // cycles     = 1
    0x00, 0x00, 0x00, 0x00,        // balanced   = 0
    0x00, 0x0C,                    // FET        = charging + discharging
    0x00, 0x00,                    // errors     = 0
    0x00, 0x64,                    // SOH        = 100 %
    0x7D,
};

// 16 cells: C1=3.288  C2=3.290  C3–C15=3.288/3.289  C16=3.286 V
// min=3.286(C16)  max=3.290(C2)  delta=0.004  avg=3.288375 V
static const std::vector<uint8_t> CELL_VOLTAGES_FRAME_1 = {
    0x7B, 0x02, 0x21,  // start | type=CELL_VOLTAGES | data_len=33
    0x10,              // cell count = 16
    0x0C, 0xD8,        // C1  = 0x0CD8 = 3288 * 0.001 = 3.288 V
    0x0C, 0xDA,        // C2  = 0x0CDA = 3290 * 0.001 = 3.290 V
    0x0C, 0xD9,        // C3  = 3.289 V
    0x0C, 0xD8,        // C4  = 3.288 V
    0x0C, 0xD8,        // C5  = 3.288 V
    0x0C, 0xD8,        // C6  = 3.288 V
    0x0C, 0xD9,        // C7  = 3.289 V
    0x0C, 0xD8,        // C8  = 3.288 V
    0x0C, 0xD8,        // C9  = 3.288 V
    0x0C, 0xD9,        // C10 = 3.289 V
    0x0C, 0xD9,        // C11 = 3.289 V
    0x0C, 0xD8,        // C12 = 3.288 V
    0x0C, 0xD9,        // C13 = 3.289 V
    0x0C, 0xD9,        // C14 = 3.289 V
    0x0C, 0xD8,        // C15 = 3.288 V
    0x0C, 0xD6,        // C16 = 0x0CD6 = 3286 * 0.001 = 3.286 V
    0x7D,
};

// 4 sensors all at 18.0 °C  (0x0B5F = 2911 -> (2911-2731)*0.1 = 18.0)
static const std::vector<uint8_t> TEMPERATURES_FRAME_1 = {
    0x7B, 0x03, 0x09,  // start | type=TEMPERATURES | data_len=9
    0x04,              // sensor count = 4
    0x0B, 0x5F,        // T1 = 0x0B5F = 2911 -> 18.0 °C
    0x0B, 0x5F,        // T2 = 18.0 °C
    0x0B, 0x5F,        // T3 = 18.0 °C
    0x0B, 0x5F,        // T4 = 18.0 °C
    0x7D,
};

// T1=13.0 °C  T2–T4=14.0 °C
// (0x0B2D=2861 -> 13.0)  (0x0B37=2871 -> 14.0)
static const std::vector<uint8_t> TEMPERATURES_FRAME_2 = {
    0x7B, 0x03, 0x09, 0x04, 0x0B, 0x2D,  // T1 = 0x0B2D = 2861 -> 13.0 °C
    0x0B, 0x37,                          // T2 = 0x0B37 = 2871 -> 14.0 °C
    0x0B, 0x37,                          // T3 = 14.0 °C
    0x0B, 0x37,                          // T4 = 14.0 °C
    0x7D,
};

// ── Manufacturing date frame (0x09) ──────────────────────────────────────────
// date unset (all 0xFF)
static const std::vector<uint8_t> MANUFACTURING_DATE_FRAME_1 = {
    0x7B, 0x09, 0x03,  // start | type=MANUFACTURING_DATE | data_len=3
    0xFF, 0xFF, 0xFF,  // year=unset  month=unset  day=unset
    0x7D,
};

// ── Model name frame (0x0A) ───────────────────────────────────────────────────
// name unset (all 0xFF)
static const std::vector<uint8_t> MODEL_NAME_FRAME_1 = {
    0x7B, 0x0A, 0x1E,                                            // start | type=MODEL_NAME | data_len=30
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // name[0..9]  = unset
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // name[10..19] = unset
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // name[20..29] = unset
    0x7D,
};

// ── Serial number frame (0x0B) ────────────────────────────────────────────────
// serial = "CER2410-018-030-023"  (19 ASCII bytes)
static const std::vector<uint8_t> SERIAL_NUMBER_FRAME_1 = {
    0x7B, 0x0B, 0x13,                                            // start | type=SERIAL_NUMBER | data_len=19
    0x43, 0x45, 0x52, 0x32, 0x34, 0x31, 0x30, 0x2D, 0x30, 0x31,  // "CER2410-01"
    0x38, 0x2D, 0x30, 0x33, 0x30, 0x2D, 0x30, 0x32, 0x33,        // "8-030-023"
    0x7D,
};

// ── Model type frame (0x0C) ───────────────────────────────────────────────────
// type unset (all 0xFF)
static const std::vector<uint8_t> MODEL_TYPE_FRAME_1 = {
    0x7B, 0x0C, 0x14,                                            // start | type=MODEL_TYPE | data_len=20
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // type[0..9]  = unset
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // type[10..19] = unset
    0x7D,
};

// ── Bluetooth software version frame (0x74) ───────────────────────────────────
// version = "KS_BLE_Ver1.0.0_20240716"  (24 ASCII bytes)
static const std::vector<uint8_t> BLUETOOTH_SW_VERSION_FRAME_1 = {
    0x7B, 0x74, 0x18,                                            // start | type=BT_SW_VERSION | data_len=24
    0x4B, 0x53, 0x5F, 0x42, 0x4C, 0x45, 0x5F, 0x56, 0x65, 0x72,  // "KS_BLE_Ver"
    0x31, 0x2E, 0x30, 0x2E, 0x30, 0x5F, 0x32, 0x30, 0x32, 0x34,  // "1.0.0_2024"
    0x30, 0x37, 0x31, 0x36,                                      // "0716"
    0x7D,
};

// ── Software version frame (0xF3) ─────────────────────────────────────────────
// 0xF3 is "No response" in the faker — frame constructed from source comments
// version = 1.5
static const std::vector<uint8_t> SOFTWARE_VERSION_FRAME_1 = {
    0x7B, 0xF3, 0x02,  // start | type=SW_VERSION | data_len=2
    0x01, 0x05,        // main=1  sub=5  -> "1.5"
    0x7D,
};

// ── Hardware version frame (0xF4) ─────────────────────────────────────────────
// version = 2.2.1
static const std::vector<uint8_t> HARDWARE_VERSION_FRAME_1 = {
    0x7B, 0xF4, 0x03,  // start | type=HW_VERSION | data_len=3
    0x02, 0x02, 0x01,  // main=2  sub=2  build=1  -> "2.2.1"
    0x7D,
};

// ── Bootloader version frame (0xF5) ───────────────────────────────────────────
// version = 2.57
static const std::vector<uint8_t> BOOTLOADER_VERSION_FRAME_1 = {
    0x7B, 0xF5, 0x02,  // start | type=BOOTLOADER_VERSION | data_len=2
    0x02, 0x39,        // main=2  sub=57  -> "2.57"
    0x7D,
};

// Invalid frames (from faker.yaml — used to test rejection logic)
static const std::vector<uint8_t> INVALID_FRAME_NO_END = {0x7B, 0xF5, 0x02, 0x02, 0x39};
static const std::vector<uint8_t> INVALID_FRAME_TOO_SHORT = {0x7B, 0xF5, 0x02, 0x02, 0x7D};
static const std::vector<uint8_t> INVALID_FRAME_TOO_LONG = {0x7B, 0xF5, 0x02, 0x02, 0x39, 0x39, 0x7D};

}  // namespace esphome::ks_bms_ble::testing
