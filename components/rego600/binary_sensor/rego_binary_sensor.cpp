#include "rego_binary_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.binary_sensor";

void RegoBinarySensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego BinarySensor:");
    LOG_BINARY_SENSOR("  ", "BinarySensor", this);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

void RegoBinarySensor::update() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->publish_state(result != 0);
    }
    else {
        ESP_LOGE(TAG, "Could not update binary sensor");
    }
}

}  // namespace rego
}  // namespace esphome
