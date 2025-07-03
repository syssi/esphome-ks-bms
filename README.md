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
[I][ks_bms_ble:198]: Status frame received
[D][ks_bms_ble:199]:   7B.01.20.00.45.14.8D.00.DC.00.B4.00.B4.00.00.52.05.75.30.75.30.00.00.00.01.00.00.00.00.00.0C.00.00.00.64.7D (36)
[D][sensor:098]: 'ks-bms-ble state of charge': Sending state 69.00000 % with 0 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble total voltage': Sending state 52.61000 V with 2 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble average temperature': Sending state 22.00000 °C with 1 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble ambient temperature': Sending state 18.00000 °C with 1 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble mosfet temperature': Sending state 18.00000 °C with 1 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble current': Sending state 0.00000 A with 2 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble power': Sending state 0.00000 W with 2 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble charging power': Sending state 0.00000 W with 2 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble discharging power': Sending state 0.00000 W with 2 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble capacity remaining': Sending state 209.97000 Ah with 2 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble full charge capacity': Sending state 300.00000 Ah with 2 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble nominal capacity': Sending state 300.00000 Ah with 2 decimals of accuracy
[D][ks_bms_ble:239]: Unknown21: 0 (0x00 0x00)
[D][sensor:098]: 'ks-bms-ble charging cycles': Sending state 1.00000  with 0 decimals of accuracy
[I][ks_bms_ble:245]: Balancer status: 0
[D][ks_bms_ble:252]: FET control status: 12 (0x00 0x0C)
[D][binary_sensor:036]: 'ks-bms-ble charging': Sending state ON
[D][binary_sensor:036]: 'ks-bms-ble discharging': Sending state ON
[D][sensor:098]: 'ks-bms-ble error bitmask': Sending state 0.00000  with 0 decimals of accuracy
[V][text_sensor:015]: 'ks-bms-ble errors': Received new state 
[D][text_sensor:069]: 'ks-bms-ble errors': Sending state ''
[D][sensor:098]: 'ks-bms-ble state of health': Sending state 100.00000 % with 0 decimals of accuracy

[I][ks_bms_ble:289]: Cell voltages frame received
[D][ks_bms_ble:290]:   7B.02.21.10.0C.D8.0C.DA.0C.D9.0C.D8.0C.D8.0C.D8.0C.D9.0C.D8.0C.D8.0C.D9.0C.D9.0C.D8.0C.D9.0C.D9.0C.D8.0C.D6.7D (37)
[D][sensor:098]: 'ks-bms-ble cell voltage 1': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 2': Sending state 3.29000 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 3': Sending state 3.28900 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 4': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 5': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 6': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 7': Sending state 3.28900 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 8': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 9': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 10': Sending state 3.28900 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 11': Sending state 3.28900 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 12': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 13': Sending state 3.28900 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 14': Sending state 3.28900 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 15': Sending state 3.28800 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble cell voltage 16': Sending state 3.28600 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble min cell voltage': Sending state 3.28600 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble max cell voltage': Sending state 3.29000 V with 3 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble min voltage cell': Sending state 16.00000  with 0 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble max voltage cell': Sending state 2.00000  with 0 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble delta cell voltage': Sending state 0.00400 V with 3 decimals of accuracy


[I][ks_bms_ble:355]: Temperatures frame received
[D][ks_bms_ble:356]:   7B.03.09.04.0B.5F.0B.5F.0B.5F.0B.5F.7D (13)
[D][sensor:098]: 'ks-bms-ble temperature 1': Sending state 18.00000 °C with 1 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble temperature 2': Sending state 18.00000 °C with 1 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble temperature 3': Sending state 18.00000 °C with 1 decimals of accuracy
[D][sensor:098]: 'ks-bms-ble temperature 4': Sending state 18.00000 °C with 1 decimals of accuracy

[I][ks_bms_ble:384]: Manufacturing date frame received
[D][ks_bms_ble:385]:   7B.09.03.FF.FF.FF.7D (7)
[I][ks_bms_ble:395]: Manufacturing date: unset

[I][ks_bms_ble:404]: Model name frame received
[D][ks_bms_ble:405]:   7B.0A.1E.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.7D (34)
[I][ks_bms_ble:415]: Model name: unset

[I][ks_bms_ble:424]: Serial number frame received
[D][ks_bms_ble:425]:   7B.0B.13.43.45.52.32.34.31.30.2D.30.31.38.2D.30.33.30.2D.30.32.33.7D (23)
[I][ks_bms_ble:436]: Serial number: CER2410-018-030-023

[I][ks_bms_ble:443]: Model type frame received
[D][ks_bms_ble:444]:   7B.0C.14.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.FF.7D (24)
[I][ks_bms_ble:453]: Model type: unset

[I][ks_bms_ble:462]: Bluetooth software version frame received
[D][ks_bms_ble:463]:   7B.74.18.4B.53.5F.42.4C.45.5F.56.65.72.31.2E.30.2E.30.5F.32.30.32.34.30.37.31.36.7D (28)
[I][ks_bms_ble:475]: Bluetooth software version: KS_BLE_Ver1.0.0_20240716

[I][ks_bms_ble:497]: Hardware version frame received
[D][ks_bms_ble:498]:   7B.F4.03.02.02.01.7D (7)
[I][ks_bms_ble:507]: Hardware version: 2.2.1

[I][ks_bms_ble:513]: Bootloader version frame received
[D][ks_bms_ble:514]:   7B.F5.02.02.39.7D (6)
[I][ks_bms_ble:522]: Bootloader version: 2.57
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
