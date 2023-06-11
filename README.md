# esphome-rego600
Started as a fork of [Husdata H60 Arduino get started code](https://github.com/peterarandis/H60-OS) 
but with a lot of influence from
[esphome/components/dsmr](https://github.com/esphome/esphome/tree/dev/esphome/components/dsmr), 
[stream server for ESPHome](https://github.com/oxan/esphome-stream-server/), 
[how to connect heat pump with Rego 6xx controller](https://rago600.sourceforge.net/) and 
[custom component esphome-rego1000](https://github.com/jagheterfredrik/esphome-rego1000)
it is now broken out as stand-alone custom component.

To be used as custom component to ESPHome

Planned functions:
  - (done) Wifi connection with DHCP and fixed SSID/PASS
  - (done) Web page showing status and heat pump data (ESPHome)
  - Basic code for communication with heat pump via ESPHome UART component
  - Provide input and output entities based on heat-pump model

Add the following to your ESPHome config

```yaml
external_components:
  - source: github://dala318/esphome-rego600

uart:
  id: uart_bus
  tx_pin: GPIO12
  rx_pin: GPIO13
  baud_rate: 19200

rego600:
  uart_id: uart_bus
  log_all: true
  id: rego600_hub

binary_sensor:
  - platform: rego600
    rego600_id: rego600_hub
    connected:
      name: Connected to Rego600

sensor:
  - platform: rego600
    rego600_id: rego600_hub
    return_temp:
      name: Return temp
    power:
      name: Power usage
```

For a complete list of available entities and registers to access, for now, refer to the code in each python entity template.

```python
CONF_DICT = {
    cv.Optional("power"): sensor.sensor_schema(ns.class_("SensorPower", sensor.Sensor, cg.Component)).extend(cv.COMPONENT_SCHEMA),
    cv.Optional("return_temp"): sensor.sensor_schema(ns.class_("SensorReturnTemp", sensor.Sensor, cg.Component)).extend(cv.COMPONENT_SCHEMA),
}
```

For debugging it's suggested to also add a [stream server](https://github.com/oxan/esphome-stream-server) to your config for direct connection between your PC and heat-pump
