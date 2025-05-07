# esphome-ks-bms

![GitHub actions](https://github.com/syssi/esphome-ks-bms/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-ks-bms)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-ks-bms)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-ks-bms)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome component to monitor a KS48100 Battery Management System via BLE

## Supported devices

* KS48100 used in PAPool/Aliexpress 48V 280Ah battery pack ([#1](https://github.com/syssi/esphome-ks-bms/issues/1))<br>
  Probably advertised via BLE as `CER2410-018-030-023` or `TZ-H002-02408-0103`

* KS48100 used in VoltPolska 12.8V 100Ah battery pack ([ref](https://dou.ua/forums/topic/42715/))<br>
  Probably advertised via BLE as `12V100A00460`

* KS48100 used in BEMORY LFP48100PB battery bank

## Requirements

* [ESPHome 2024.6.0 or higher](https://github.com/esphome/esphome/releases).
* Generic ESP32 board

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://syssi/esphome-ks-bms@main
```

or just use the `esp32-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-ks-bms.git
cd esphome-ks-bms

# Create a secrets.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
bms0_mac_address: MY_BMS_MAC_ADDRESS

wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD

mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
esphome run esp32-ble-example.yaml

```

## Example response all sensors enabled

```
TBD.
```

## Protocol

TBD.

## Known issues

None.

## Debugging

If this component doesn't work out of the box for your device please update your configuration to increase the log level to see details about the BLE communication and incoming traffic:

```
logger:
  level: VERY_VERBOSE
  logs:
    esp32_ble: DEBUG
    esp32_ble_tracker: VERY_VERBOSE
    ks_bms_ble: VERY_VERBOSE
    scheduler: DEBUG
    component: DEBUG
    sensor: DEBUG
    mqtt: INFO
    mqtt.idf: INFO
    mqtt.component: INFO
    mqtt.sensor: INFO
    mqtt.switch: INFO
    api.service: INFO
    api: INFO
    api: INFO
```

## References

* https://github.com/ALEGATOR1209/VoltPolska/blob/main/app/src/main/java/ua/alegator1209/voltpolska/data/DeviceRepository.kt
* https://dou.ua/forums/topic/42715/
* https://diysolarforum.com/threads/bemory-bms-ks48100.87902/
