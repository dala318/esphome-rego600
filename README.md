# H60-OS
Husdata H60 Arduino get started code
HUSDATA.SE, Peter Hansson. h60@husdata.se

Open H60 Arduino code as a foundation for own development projects on the H60 platform
Provided as-is with no support. Can be freely used in personal och commercial projects.

Functions:
  - Wifi connection with DHCP and fixed SSID/PASS  
  - Basic code for communication with heat pump
  - Web page showing status and heat pump data
  - restAPI at  /api/alldata
  - LED status H60 red/green
  - Routine for button pushed
  - Debug printout via serial/usb port

Add the following to your ESPHome config

```yaml
external_components:
  # - source: https://github.com/peterarandis/H60-OS

uart:
  id: uart_bus
  tx_pin: GPIO12
  rx_pin: GPIO13
  baud_rate: 19200

h60_interface:
  uart_id: uart_bus
```
