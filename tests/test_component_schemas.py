"""Schema structure tests for ks_bms_ble ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.ks_bms_ble as hub  # noqa: E402
from components.ks_bms_ble import (  # noqa: E402
    binary_sensor,
    number,
    sensor,
    switch,  # noqa: E402
    text_sensor,
)


class TestHubConstants:
    def test_conf_ks_bms_ble_id_defined(self):
        assert hub.CONF_KS_BMS_BLE_ID == "ks_bms_ble_id"


class TestSensorLists:
    def test_cells_count(self):
        assert len(sensor.CELLS) == 24

    def test_cells_naming(self):
        assert sensor.CELLS[0] == "cell_voltage_1"
        assert sensor.CELLS[23] == "cell_voltage_24"
        for i, key in enumerate(sensor.CELLS, 1):
            assert key == f"cell_voltage_{i}"

    def test_temperatures_count(self):
        assert len(sensor.TEMPERATURES) == 8

    def test_temperatures_naming(self):
        assert sensor.TEMPERATURES[0] == "temperature_1"
        assert sensor.TEMPERATURES[7] == "temperature_8"
        for i, key in enumerate(sensor.TEMPERATURES, 1):
            assert key == f"temperature_{i}"

    def test_sensor_defs_completeness(self):
        assert sensor.CONF_TOTAL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_STATE_OF_CHARGE in sensor.SENSOR_DEFS
        assert sensor.CONF_MIN_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_MAX_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_DELTA_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_AVERAGE_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_STATE_OF_HEALTH in sensor.SENSOR_DEFS
        assert len(sensor.SENSOR_DEFS) == 37

    def test_no_cell_keys_in_sensor_defs(self):
        for key in sensor.SENSOR_DEFS:
            assert key not in sensor.CELLS
            assert key not in sensor.TEMPERATURES


class TestBinarySensorDefs:
    def test_binary_sensor_defs_completeness(self):
        assert binary_sensor.CONF_CHARGING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_DISCHARGING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_LIMITING_CURRENT in binary_sensor.BINARY_SENSOR_DEFS
        assert len(binary_sensor.BINARY_SENSOR_DEFS) == 3


class TestTextSensors:
    def test_text_sensors_list(self):
        assert text_sensor.CONF_SOFTWARE_VERSION in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_HARDWARE_VERSION in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_BOOTLOADER_VERSION in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_SERIAL_NUMBER in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_MANUFACTURING_DATE in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_MODEL_TYPE in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_BLUETOOTH_SOFTWARE_VERSION in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_DEVICE_MODEL in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_ERRORS in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_BALANCER_STATUS in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 13


class TestSwitchConstants:
    def test_switches_dict(self):
        assert switch.CONF_CHARGING in switch.SWITCHES
        assert switch.CONF_DISCHARGING in switch.SWITCHES
        assert len(switch.SWITCHES) == 2

    def test_switch_addresses_are_unique(self):
        addresses = list(switch.SWITCHES.values())
        assert len(addresses) == len(set(addresses))


class TestNumberConstants:
    def test_numbers_dict_populated(self):
        assert len(number.NUMBERS) > 0

    def test_numbers_addresses_are_unique(self):
        addresses = [v[0] for v in number.NUMBERS.values()]
        assert len(addresses) == len(set(addresses))

    def test_cell_voltage_numbers_present(self):
        assert "cell_full_voltage" in number.NUMBERS
        assert "cell_cutoff_voltage" in number.NUMBERS

    def test_voltage_protection_numbers_present(self):
        assert "cell_overvoltage_protection" in number.NUMBERS
        assert "cell_overvoltage_recovery" in number.NUMBERS
        assert "cell_overvoltage_protection_delay" in number.NUMBERS
        assert "cell_undervoltage_protection" in number.NUMBERS
        assert "cell_undervoltage_recovery" in number.NUMBERS
        assert "cell_undervoltage_protection_delay" in number.NUMBERS

    def test_temperature_protection_numbers_present(self):
        assert "charge_overtemperature_protection" in number.NUMBERS
        assert "charge_overtemperature_recovery" in number.NUMBERS
        assert "charge_overtemperature_protection_delay" in number.NUMBERS
        assert "charge_undertemperature_protection" in number.NUMBERS
        assert "charge_undertemperature_recovery" in number.NUMBERS
        assert "charge_undertemperature_protection_delay" in number.NUMBERS
        assert "discharge_overtemperature_protection" in number.NUMBERS
        assert "discharge_overtemperature_recovery" in number.NUMBERS
        assert "discharge_overtemperature_protection_delay" in number.NUMBERS
        assert "discharge_undertemperature_protection" in number.NUMBERS
        assert "discharge_undertemperature_recovery" in number.NUMBERS
        assert "discharge_undertemperature_protection_delay" in number.NUMBERS

    def test_current_protection_numbers_present(self):
        assert "charge_overcurrent_protection" in number.NUMBERS
        assert "charge_overcurrent_protection_delay" in number.NUMBERS
        assert "charge_overcurrent_recovery_delay" in number.NUMBERS
        assert "discharge_overcurrent_protection" in number.NUMBERS
        assert "discharge_overcurrent_protection_delay" in number.NUMBERS
        assert "discharge_overcurrent_recovery_delay" in number.NUMBERS
