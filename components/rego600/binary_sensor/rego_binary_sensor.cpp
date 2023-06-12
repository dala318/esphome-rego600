#include "rego_binary_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "Rego.BinarySensor";

void RegoBinarySensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego BinarySensor:");
    LOG_BINARY_SENSOR("  ", "BinarySensor", this);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

}  // namespace rego
}  // namespace esphome
