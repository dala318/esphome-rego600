# esphome-rego600
Fork of [Husdata H60 Arduino get started code](https://github.com/peterarandis/H60-OS) 
with a lot of influence from [esphome/components/dsmr](https://github.com/esphome/esphome/tree/dev/esphome/components/dsmr)

To be used as custom component to ESPHome

Planned functions:
  - Wifi connection with DHCP and fixed SSID/PASS (ESPHome)
  - Web page showing status and heat pump data (ESPHome)
  - Basic code for communication with heat pump
  - Provide inut and output entities based on heat-pump model

For contributors please refer to the following projects:

https://rago600.sourceforge.net/

https://github.com/peterarandis/H60-OS

https://github.com/jagheterfredrik/esphome-rego1000

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
  id: rego600_hub

sensor:
  - platform: rego600
    rego600_id: rego600_hub
    return_temp:
      name: Return temp
```

For debugging it's suggested to also add a [stream server](https://github.com/oxan/esphome-stream-server) to your config for direct connection between your PC and heat-pump
