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
  using KsBmsBle::decode_current_protection_data_;
  using KsBmsBle::decode_hardware_version_data_;
  using KsBmsBle::decode_manufacturing_date_data_;
  using KsBmsBle::decode_model_name_data_;
  using KsBmsBle::decode_model_type_data_;
  using KsBmsBle::decode_serial_number_data_;
  using KsBmsBle::decode_software_version_data_;
  using KsBmsBle::decode_basic_config_data_;
  using KsBmsBle::decode_status_data_;
  using KsBmsBle::decode_temperatures_data_;
  using KsBmsBle::decode_temperature_protection_data_;
  using KsBmsBle::decode_voltage_protection_data_;
  using KsBmsBle::on_ks_bms_ble_data;
};

// Concrete Switch implementation for testing (write_state is pure virtual).
class TestSwitch : public switch_::Switch {
 protected:
  void write_state(bool state) override { this->publish_state(state); }
};

// Concrete Number implementation for testing (control is pure virtual).
class TestNumber : public number::Number {
 protected:
  void control(float value) override { this->publish_state(value); }
};

// ── Real frames from tests/esp32-ble-example-faker-ks48300.yaml ─────────────────
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

// empty response (data_len=0) - BL-250-100-100 esphome.log 2026-04-28
static const std::vector<uint8_t> MODEL_NAME_FRAME_EMPTY = {
    0x7B,
    0x0A,
    0x00,  // start | type=MODEL_NAME | data_len=0
    0x7D,
};

// date: 2024-10-15 - synthetic test fixture
static const std::vector<uint8_t> MANUFACTURING_DATE_FRAME_2 = {
    0x7B, 0x09, 0x03,  // start | type=MANUFACTURING_DATE | data_len=3
    0x18, 0x0A, 0x0F,  // year=24+2000=2024  month=10  day=15
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

// serial = "BL-250-100-100"  (14 ASCII bytes) - BL-250-100-100 esphome.log 2026-04-28
static const std::vector<uint8_t> SERIAL_NUMBER_FRAME_2 = {
    0x7B, 0x0B, 0x0E,  // start | type=SERIAL_NUMBER | data_len=14
    0x42, 0x4C, 0x2D, 0x32, 0x35, 0x30, 0x2D, 0x31, 0x30, 0x30, 0x2D, 0x31, 0x30, 0x30,  // "BL-250-100-100"
    0x7D,
};

// ── Model type frame (0x0C) ───────────────────────────────────────────────────
// type = "KS48300"  (7 ASCII bytes) - synthetic test fixture
static const std::vector<uint8_t> MODEL_TYPE_FRAME_2 = {
    0x7B, 0x0C, 0x07,                          // start | type=MODEL_TYPE | data_len=7
    0x4B, 0x53, 0x34, 0x38, 0x33, 0x30, 0x30,  // "KS48300"
    0x7D,
};

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

// WiFi variant: version = "KS_BLE_WIFI_Ver1.0.0_20250529"  (29 ASCII bytes, data_len=0x1D)
// Real frame from tests/esp32-ble-example-faker-ks48300.yaml - different length than non-WiFi variant
static const std::vector<uint8_t> BLUETOOTH_SW_VERSION_FRAME_2 = {
    0x7B, 0x74, 0x1D,                                            // start | type=BT_SW_VERSION | data_len=29
    0x4B, 0x53, 0x5F, 0x42, 0x4C, 0x45, 0x5F, 0x57, 0x49, 0x46,  // "KS_BLE_WiF"
    0x49, 0x5F, 0x56, 0x65, 0x72, 0x31, 0x2E, 0x30, 0x2E, 0x30,  // "i_Ver1.0.0"
    0x5F, 0x32, 0x30, 0x32, 0x35, 0x30, 0x35, 0x32, 0x39,        // "_20250529"
    0x7D,
};

// ── Software version frame (0xF3) ─────────────────────────────────────────────
// 0xF3 is "No response" in the faker - frame constructed from source comments
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

// ── Voltage protection frame (0x05) ───────────────────────────────────────────
// Real frame from tests/esp32-ble-example-faker-ks48300.yaml
// cell_ovp=3.648V, cell_ovr=3.380V, delay=2.0s, cell_uvp=2.50V, cell_uvr=3.00V
// pack_ovp=58.4V, pack_ovr=54.4V, delay=2.0s, pack_uvp=40.0V, pack_uvr=48.0V
static const std::vector<uint8_t> VOLTAGE_PROTECTION_FRAME_1 = {
    0x7B, 0x05, 0x18,  // start | type=VOLTAGE_PROTECTION | data_len=24
    0x0E, 0x42,        // cell_overvoltage_protection   = 3648/1000 = 3.648 V
    0x0D, 0x34,        // cell_overvoltage_recovery     = 3380/1000 = 3.380 V
    0x07, 0xD0,        // cell_overvoltage_protection_delay = 2000/1000 = 2.0 s
    0x09, 0xC4,        // cell_undervoltage_protection  = 2500/1000 = 2.50 V
    0x0B, 0xB8,        // cell_undervoltage_recovery    = 3000/1000 = 3.00 V
    0x13, 0x88,        // cell_undervoltage_protection_delay = 5000/1000 = 5.0 s
    0x16, 0xD0,        // pack_overvoltage_protection   = 5840/100 = 58.4 V
    0x15, 0x40,        // pack_overvoltage_recovery     = 5440/100 = 54.4 V
    0x07, 0xD0,        // pack_overvoltage_protection_delay = 2000/1000 = 2.0 s
    0x0F, 0xA0,        // pack_undervoltage_protection = 4000/100 = 40.0 V
    0x12, 0xC0,        // pack_undervoltage_recovery    = 4800/100 = 48.0 V
    0x07, 0xD0,        // pack_undervoltage_protection_delay = 2000/1000 = 2.0 s
    0x7D,
};

// ── Temperature protection frame (0x06) ─────────────────────────────────────────
// Real frame from tests/esp32-ble-example-faker-ks48300.yaml
// charge 50/45/3°C, 20/10/5°C, discharge 65/55/3°C, -10/0/5°C
// Kelvin encoding: raw = °C * 10 + 2731
static const std::vector<uint8_t> TEMPERATURE_PROTECTION_FRAME_1 = {
    0x7B, 0x06, 0x18,  // start | type=TEMPERATURE_PROTECTION | data_len=24
    0x0C, 0xA3,        // charge_overtemperature_protection    = 50°C
    0x0C, 0x6D,        // charge_overtemperature_recovery      = 45°C
    0x00, 0x03,        // charge_overtemperature_protection_delay = 3.0 s
    0x0B, 0x73,        // charge_undertemperature_protection   = 20°C
    0x0B, 0x0F,        // charge_undertemperature_recovery      = 10°C
    0x00, 0x05,        // charge_undertemperature_protection_delay = 5.0 s
    0x0D, 0x35,        // discharge_overtemperature_protection = 65°C
    0x0C, 0xC1,        // discharge_overtemperature_recovery   = 55°C
    0x00, 0x03,        // discharge_overtemperature_protection_delay = 3.0 s
    0x09, 0xE3,        // discharge_undertemperature_protection = -10°C
    0x0A, 0xAB,        // discharge_undertemperature_recovery  = 0°C
    0x00, 0x05,        // discharge_undertemperature_protection_delay = 5.0 s
    0x7D,
};

// ── Current protection frame (0x07) ────────────────────────────────────────────
// Real frame from tests/esp32-ble-example-faker-ks48300.yaml
// charge 314.68A/3.083s/30s, discharge 1.23A/3.083s/30s
static const std::vector<uint8_t> CURRENT_PROTECTION_FRAME_1 = {
    0x7B, 0x07, 0x0C,  // start | type=CURRENT_PROTECTION | data_len=12
    0x7B, 0x0C,        // charge_overcurrent_protection = 31468/100 = 314.68 A
    0x0C, 0x0B,        // charge_overcurrent_protection_delay = 3083/1000 = 3.083 s
    0x0B, 0xB8,        // charge_overcurrent_recovery_delay = 3000/100 = 30.0 s
    0x00, 0x7B,        // discharge_overcurrent_protection = 123/100 = 1.23 A
    0x0C, 0x0B,        // discharge_overcurrent_protection_delay = 3083/1000 = 3.083 s
    0x0B, 0xB8,        // discharge_overcurrent_recovery_delay = 3000/100 = 30.0 s
    0x7D,
};

// ── Basic config frame (0x04) ─────────────────────────────────────────────────
// cell_full=3.650V  cell_cutoff=3.000V  bal_open=3.350V  bal_diff=0.010V
// reg0x14=0  reg0x15=1.00
// soc80=3.450V  soc60=3.380V  soc40=3.320V  soc20=3.250V
static const std::vector<uint8_t> BASIC_CONFIG_FRAME_1 = {
    0x7B, 0x04, 0x14,  // start | type=BASIC_CONFIG | data_len=20
    0x0E, 0x42,        // cell_full_voltage        = 3650/1000 = 3.650 V  (reg 0x10)
    0x0B, 0xB8,        // cell_cutoff_voltage      = 3000/1000 = 3.000 V  (reg 0x11)
    0x0D, 0x16,        // balance_open_voltage     = 3350/1000 = 3.350 V  (reg 0x12)
    0x00, 0x0A,        // balance_open_voltage_diff=   10/1000 = 0.010 V  (reg 0x13)
    0x00, 0x00,        // unknown                  = 0                    (reg 0x14)
    0x00, 0x64,        // unknown                  = 100/100  = 1.00      (reg 0x15)
    0x0D, 0x7A,        // soc_80_voltage           = 3450/1000 = 3.450 V  (reg 0x16)
    0x0D, 0x34,        // soc_60_voltage           = 3380/1000 = 3.380 V  (reg 0x17)
    0x0C, 0xF8,        // soc_40_voltage           = 3320/1000 = 3.320 V  (reg 0x18)
    0x0C, 0xB2,        // soc_20_voltage           = 3250/1000 = 3.250 V  (reg 0x19)
    0x7D,
};

// BL-250-100-100-2: cell_full=3.500V  cell_cutoff=2.500V  bal_open=3.450V  bal_diff=0.300V
// all SoC voltages = 0 (not configured on this device)
static const std::vector<uint8_t> BASIC_CONFIG_FRAME_2 = {
    0x7B, 0x04, 0x14,  // start | type=BASIC_CONFIG | data_len=20
    0x0D, 0xAC,        // cell_full_voltage        = 3500/1000 = 3.500 V  (reg 0x10)
    0x09, 0xC4,        // cell_cutoff_voltage      = 2500/1000 = 2.500 V  (reg 0x11)
    0x0D, 0x7A,        // balance_open_voltage     = 3450/1000 = 3.450 V  (reg 0x12)
    0x01, 0x2C,        // balance_open_voltage_diff=  300/1000 = 0.300 V  (reg 0x13)
    0x00, 0x00,        // unknown                  = 0                    (reg 0x14)
    0x00, 0x00,        // unknown                  = 0                    (reg 0x15)
    0x00, 0x00,        // soc_80_voltage           = 0.000 V              (reg 0x16)
    0x00, 0x00,        // soc_60_voltage           = 0.000 V              (reg 0x17)
    0x00, 0x00,        // soc_40_voltage           = 0.000 V              (reg 0x18)
    0x00, 0x00,        // soc_20_voltage           = 0.000 V              (reg 0x19)
    0x7D,
};

// Wrong length byte - must be rejected by the guard in decode_basic_config_data_
static const std::vector<uint8_t> BASIC_CONFIG_FRAME_INVALID_LEN = {
    0x7B, 0x04, 0x1A,  // length 0x1A != 0x14 → guard rejects
    0x0E, 0x42, 0x0B, 0xB8, 0x0D, 0x16, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D,
};

// Invalid frames (from tests/esp32-ble-example-debug.yaml - used to test rejection logic)
static const std::vector<uint8_t> INVALID_FRAME_NO_END = {0x7B, 0xF5, 0x02, 0x02, 0x39};
static const std::vector<uint8_t> INVALID_FRAME_TOO_SHORT = {0x7B, 0xF5, 0x02, 0x02, 0x7D};
static const std::vector<uint8_t> INVALID_FRAME_TOO_LONG = {0x7B, 0xF5, 0x02, 0x02, 0x39, 0x39, 0x7D};

}  // namespace esphome::ks_bms_ble::testing
