# H60-OS
Follow up to Husdata H60 Arduino get started code (https://github.com/peterarandis/H60-OS)

To be used as custom component to ESPHome

Functions:
  - Wifi connection with DHCP and fixed SSID/PASS  
  - Basic code for communication with heat pump
  - Web page showing status and heat pump data (ESPHome)
  - LED status H60 red/green
  - Routine for button pushed

Add the following to your ESPHome config

```yaml
external_components:
  - source: github://dala318/esphome-h60-interface

uart:
  id: uart_bus
  tx_pin: GPIO12
  rx_pin: GPIO13
  baud_rate: 19200

h60_interface:
  uart_id: uart_bus
```

For debugging it's suggested to also add a [stream server](https://github.com/oxan/esphome-stream-server) to your config for direct connection between your PC and heat-pump
