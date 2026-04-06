#include <gtest/gtest.h>
#include "common.h"

namespace esphome::ks_bms_ble::testing {

// ── Status frame (0x01) ───────────────────────────────────────────────────────

TEST(KsBmsBleStatusTest, StateOfCharge) {
  TestableKsBmsBle bms;
  sensor::Sensor soc;
  bms.set_state_of_charge_sensor(&soc);

  bms.decode_status_data_(STATUS_FRAME_1);

  EXPECT_FLOAT_EQ(soc.state, 69.0f);
}

TEST(KsBmsBleStatusTest, TotalVoltage) {
  TestableKsBmsBle bms;
  sensor::Sensor voltage;
  bms.set_total_voltage_sensor(&voltage);

  bms.decode_status_data_(STATUS_FRAME_1);

  EXPECT_NEAR(voltage.state, 52.61f, 0.001f);
}

TEST(KsBmsBleStatusTest, Temperatures) {
  TestableKsBmsBle bms;
  sensor::Sensor avg, ambient, mosfet;
  bms.set_average_temperature_sensor(&avg);
  bms.set_ambient_temperature_sensor(&ambient);
  bms.set_mosfet_temperature_sensor(&mosfet);

  bms.decode_status_data_(STATUS_FRAME_1);

  EXPECT_NEAR(avg.state, 22.0f, 0.01f);
  EXPECT_NEAR(ambient.state, 18.0f, 0.01f);
  EXPECT_NEAR(mosfet.state, 18.0f, 0.01f);
}

TEST(KsBmsBleStatusTest, TemperaturesSecondFrame) {
  TestableKsBmsBle bms;
  sensor::Sensor avg, ambient, mosfet;
  bms.set_average_temperature_sensor(&avg);
  bms.set_ambient_temperature_sensor(&ambient);
  bms.set_mosfet_temperature_sensor(&mosfet);

  bms.decode_status_data_(STATUS_FRAME_2);

  EXPECT_NEAR(avg.state, 18.0f, 0.01f);
  EXPECT_NEAR(ambient.state, 13.7f, 0.01f);
  EXPECT_NEAR(mosfet.state, 14.0f, 0.01f);
}

TEST(KsBmsBleStatusTest, ZeroCurrentMeansZeroPower) {
  TestableKsBmsBle bms;
  sensor::Sensor current, power, charging_power, discharging_power;
  bms.set_current_sensor(&current);
  bms.set_power_sensor(&power);
  bms.set_charging_power_sensor(&charging_power);
  bms.set_discharging_power_sensor(&discharging_power);

  bms.decode_status_data_(STATUS_FRAME_1);  // current = 0A

  EXPECT_FLOAT_EQ(current.state, 0.0f);
  EXPECT_FLOAT_EQ(power.state, 0.0f);
  EXPECT_FLOAT_EQ(charging_power.state, 0.0f);
  EXPECT_FLOAT_EQ(discharging_power.state, 0.0f);
}

TEST(KsBmsBleStatusTest, Capacities) {
  TestableKsBmsBle bms;
  sensor::Sensor remaining, full, nominal;
  bms.set_capacity_remaining_sensor(&remaining);
  bms.set_full_charge_capacity_sensor(&full);
  bms.set_nominal_capacity_sensor(&nominal);

  bms.decode_status_data_(STATUS_FRAME_1);

  EXPECT_NEAR(remaining.state, 209.97f, 0.001f);
  EXPECT_NEAR(full.state, 300.00f, 0.001f);
  EXPECT_NEAR(nominal.state, 300.00f, 0.001f);
}

TEST(KsBmsBleStatusTest, CapacitiesSecondFrame) {
  TestableKsBmsBle bms;
  sensor::Sensor remaining;
  bms.set_capacity_remaining_sensor(&remaining);

  bms.decode_status_data_(STATUS_FRAME_2);

  EXPECT_NEAR(remaining.state, 209.92f, 0.001f);
}

TEST(KsBmsBleStatusTest, CyclesAndStateOfHealth) {
  TestableKsBmsBle bms;
  sensor::Sensor cycles, soh;
  bms.set_charging_cycles_sensor(&cycles);
  bms.set_state_of_health_sensor(&soh);

  bms.decode_status_data_(STATUS_FRAME_1);

  EXPECT_FLOAT_EQ(cycles.state, 1.0f);
  EXPECT_FLOAT_EQ(soh.state, 100.0f);
}

TEST(KsBmsBleStatusTest, NoBalancingActive) {
  TestableKsBmsBle bms;
  sensor::Sensor balanced_cell, balanced_bitmask;
  bms.set_balanced_cell_sensor(&balanced_cell);
  bms.set_balanced_cell_bitmask_sensor(&balanced_bitmask);

  bms.decode_status_data_(STATUS_FRAME_1);  // balanced bitmask = 0x00000000

  EXPECT_FLOAT_EQ(balanced_bitmask.state, 0.0f);
  EXPECT_FLOAT_EQ(balanced_cell.state, 0.0f);
}

TEST(KsBmsBleStatusTest, FetControlChargingAndDischarging) {
  TestableKsBmsBle bms;
  binary_sensor::BinarySensor charging, discharging;
  bms.set_charging_binary_sensor(&charging);
  bms.set_discharging_binary_sensor(&discharging);

  bms.decode_status_data_(STATUS_FRAME_1);  // FET = 0x000C: bit2=charging bit3=discharging

  EXPECT_TRUE(charging.state);
  EXPECT_TRUE(discharging.state);
}

TEST(KsBmsBleStatusTest, SwitchesMatchBinarySensors) {
  TestableKsBmsBle bms;
  binary_sensor::BinarySensor charging_bs, discharging_bs;
  TestSwitch charging_sw, discharging_sw;
  bms.set_charging_binary_sensor(&charging_bs);
  bms.set_discharging_binary_sensor(&discharging_bs);
  bms.set_charging_switch(&charging_sw);
  bms.set_discharging_switch(&discharging_sw);

  bms.decode_status_data_(STATUS_FRAME_1);  // FET = 0x000C: charging + discharging

  EXPECT_EQ(charging_sw.state, charging_bs.state);
  EXPECT_EQ(discharging_sw.state, discharging_bs.state);
}

TEST(KsBmsBleStatusTest, NoFaults) {
  TestableKsBmsBle bms;
  sensor::Sensor error_bitmask;
  text_sensor::TextSensor errors_text;
  bms.set_error_bitmask_sensor(&error_bitmask);
  bms.set_errors_text_sensor(&errors_text);

  bms.decode_status_data_(STATUS_FRAME_1);  // errors = 0x0000

  EXPECT_FLOAT_EQ(error_bitmask.state, 0.0f);
  EXPECT_EQ(errors_text.state, "");
}

TEST(KsBmsBleStatusTest, NullSensorsDoNotCrash) {
  TestableKsBmsBle bms;
  bms.decode_status_data_(STATUS_FRAME_1);  // must not crash with all sensors null
}

// ── Frame dispatch and validation ─────────────────────────────────────────────

TEST(KsBmsBleDispatchTest, StatusFrameDispatchedViaOnData) {
  TestableKsBmsBle bms;
  sensor::Sensor soc;
  bms.set_state_of_charge_sensor(&soc);

  bms.on_ks_bms_ble_data(0, STATUS_FRAME_1);

  EXPECT_FLOAT_EQ(soc.state, 69.0f);
}

TEST(KsBmsBleDispatchTest, InvalidFrameMissingEndByte) {
  TestableKsBmsBle bms;
  sensor::Sensor soc;
  soc.state = -1.0f;
  bms.set_state_of_charge_sensor(&soc);

  bms.on_ks_bms_ble_data(0, INVALID_FRAME_NO_END);

  EXPECT_FLOAT_EQ(soc.state, -1.0f);  // unchanged — frame rejected
}

TEST(KsBmsBleDispatchTest, InvalidFrameLengthMismatch) {
  TestableKsBmsBle bms;
  sensor::Sensor soc;
  soc.state = -1.0f;
  bms.set_state_of_charge_sensor(&soc);

  bms.on_ks_bms_ble_data(0, INVALID_FRAME_TOO_SHORT);

  EXPECT_FLOAT_EQ(soc.state, -1.0f);
}

TEST(KsBmsBleDispatchTest, InvalidFrameTooLong) {
  TestableKsBmsBle bms;
  sensor::Sensor soc;
  soc.state = -1.0f;
  bms.set_state_of_charge_sensor(&soc);

  bms.on_ks_bms_ble_data(0, INVALID_FRAME_TOO_LONG);

  EXPECT_FLOAT_EQ(soc.state, -1.0f);
}

// ── Cell voltages frame (0x02) ────────────────────────────────────────────────

TEST(KsBmsBleVoltagesTest, CellVoltagesSampled) {
  TestableKsBmsBle bms;
  sensor::Sensor c1, c2, c16;
  bms.set_cell_voltage_sensor(0, &c1);
  bms.set_cell_voltage_sensor(1, &c2);
  bms.set_cell_voltage_sensor(15, &c16);

  bms.decode_cell_voltages_data_(CELL_VOLTAGES_FRAME_1);

  EXPECT_NEAR(c1.state, 3.288f, 0.0001f);
  EXPECT_NEAR(c2.state, 3.290f, 0.0001f);
  EXPECT_NEAR(c16.state, 3.286f, 0.0001f);
}

TEST(KsBmsBleVoltagesTest, MinCellIsC16MaxCellIsC2) {
  TestableKsBmsBle bms;
  sensor::Sensor vmin, vmax, vmin_cell, vmax_cell;
  bms.set_min_cell_voltage_sensor(&vmin);
  bms.set_max_cell_voltage_sensor(&vmax);
  bms.set_min_voltage_cell_sensor(&vmin_cell);
  bms.set_max_voltage_cell_sensor(&vmax_cell);

  bms.decode_cell_voltages_data_(CELL_VOLTAGES_FRAME_1);

  EXPECT_NEAR(vmin.state, 3.286f, 0.0001f);
  EXPECT_FLOAT_EQ(vmin_cell.state, 16.0f);
  EXPECT_NEAR(vmax.state, 3.290f, 0.0001f);
  EXPECT_FLOAT_EQ(vmax_cell.state, 2.0f);
}

TEST(KsBmsBleVoltagesTest, DeltaAndAverage) {
  TestableKsBmsBle bms;
  sensor::Sensor delta, avg;
  bms.set_delta_cell_voltage_sensor(&delta);
  bms.set_average_cell_voltage_sensor(&avg);

  bms.decode_cell_voltages_data_(CELL_VOLTAGES_FRAME_1);

  EXPECT_NEAR(delta.state, 0.004f, 0.0001f);
  EXPECT_NEAR(avg.state, 3.288375f, 0.0001f);
}

TEST(KsBmsBleVoltagesTest, NullSensorsDoNotCrash) {
  TestableKsBmsBle bms;
  bms.decode_cell_voltages_data_(CELL_VOLTAGES_FRAME_1);  // must not crash with all sensors null
}

// ── Temperatures frame (0x03) ─────────────────────────────────────────────────

TEST(KsBmsBleTemperaturesTest, AllSensorsEighteenDegrees) {
  TestableKsBmsBle bms;
  sensor::Sensor t1, t2, t3, t4;
  bms.set_temperature_sensor(0, &t1);
  bms.set_temperature_sensor(1, &t2);
  bms.set_temperature_sensor(2, &t3);
  bms.set_temperature_sensor(3, &t4);

  bms.decode_temperatures_data_(TEMPERATURES_FRAME_1);

  EXPECT_NEAR(t1.state, 18.0f, 0.1f);
  EXPECT_NEAR(t2.state, 18.0f, 0.1f);
  EXPECT_NEAR(t3.state, 18.0f, 0.1f);
  EXPECT_NEAR(t4.state, 18.0f, 0.1f);
}

TEST(KsBmsBleTemperaturesTest, MixedTemperatures) {
  TestableKsBmsBle bms;
  sensor::Sensor t1, t2;
  bms.set_temperature_sensor(0, &t1);
  bms.set_temperature_sensor(1, &t2);

  bms.decode_temperatures_data_(TEMPERATURES_FRAME_2);  // T1=13.0°C  T2=14.0°C

  EXPECT_NEAR(t1.state, 13.0f, 0.1f);
  EXPECT_NEAR(t2.state, 14.0f, 0.1f);
}

TEST(KsBmsBleTemperaturesTest, NullSensorsDoNotCrash) {
  TestableKsBmsBle bms;
  bms.decode_temperatures_data_(TEMPERATURES_FRAME_1);  // must not crash with all sensors null
}

// ── Manufacturing date frame (0x09) ──────────────────────────────────────────

TEST(KsBmsBleManufacturingDateTest, UnsetDate) {
  TestableKsBmsBle bms;
  bms.decode_manufacturing_date_data_(MANUFACTURING_DATE_FRAME_1);  // year/month/day all 0xFF -> "unset" branch
}

TEST(KsBmsBleManufacturingDateTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, MANUFACTURING_DATE_FRAME_1);
}

// ── Model name frame (0x0A) ───────────────────────────────────────────────────

TEST(KsBmsBleModelNameTest, UnsetName) {
  TestableKsBmsBle bms;
  bms.decode_model_name_data_(MODEL_NAME_FRAME_1);  // all 0xFF -> "unset" branch
}

TEST(KsBmsBleModelNameTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, MODEL_NAME_FRAME_1);
}

// ── Serial number frame (0x0B) ────────────────────────────────────────────────

TEST(KsBmsBleSerialNumberTest, SetSerialNumber) {
  TestableKsBmsBle bms;
  bms.decode_serial_number_data_(SERIAL_NUMBER_FRAME_1);  // "CER2410-018-030-023" -> non-unset branch
}

TEST(KsBmsBleSerialNumberTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, SERIAL_NUMBER_FRAME_1);
}

// ── Model type frame (0x0C) ───────────────────────────────────────────────────

TEST(KsBmsBleModelTypeTest, UnsetType) {
  TestableKsBmsBle bms;
  bms.decode_model_type_data_(MODEL_TYPE_FRAME_1);  // all 0xFF -> "unset" branch
}

TEST(KsBmsBleModelTypeTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, MODEL_TYPE_FRAME_1);
}

// ── Bluetooth software version frame (0x74) ───────────────────────────────────

TEST(KsBmsBleBtSwVersionTest, KsBleVer100) {
  TestableKsBmsBle bms;
  bms.decode_bluetooth_software_version_data_(BLUETOOTH_SW_VERSION_FRAME_1);  // "KS_BLE_Ver1.0.0_20240716"
}

TEST(KsBmsBleBtSwVersionTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, BLUETOOTH_SW_VERSION_FRAME_1);
}

// ── Software version frame (0xF3) ─────────────────────────────────────────────

TEST(KsBmsBleSwVersionTest, Version1Dot5) {
  TestableKsBmsBle bms;
  bms.decode_software_version_data_(SOFTWARE_VERSION_FRAME_1);  // 1.5
}

TEST(KsBmsBleSwVersionTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, SOFTWARE_VERSION_FRAME_1);
}

// ── Hardware version frame (0xF4) ─────────────────────────────────────────────

TEST(KsBmsBleHwVersionTest, Version2Dot2Dot1) {
  TestableKsBmsBle bms;
  bms.decode_hardware_version_data_(HARDWARE_VERSION_FRAME_1);  // 2.2.1
}

TEST(KsBmsBleHwVersionTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, HARDWARE_VERSION_FRAME_1);
}

// ── Bootloader version frame (0xF5) ───────────────────────────────────────────

TEST(KsBmsBleBootloaderVersionTest, Version2Dot57) {
  TestableKsBmsBle bms;
  bms.decode_bootloader_version_data_(BOOTLOADER_VERSION_FRAME_1);  // 2.57
}

TEST(KsBmsBleBootloaderVersionTest, DispatchedViaOnData) {
  TestableKsBmsBle bms;
  bms.on_ks_bms_ble_data(0, BOOTLOADER_VERSION_FRAME_1);
}

}  // namespace esphome::ks_bms_ble::testing
