#include "rego_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.sensor";

void RegoSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Sensor:");
    LOG_SENSOR("  ", "Sensor", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int2hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Value factor: %f", this->value_factor_);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

void RegoSensor::update() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->publish_state(result * this->value_factor_);
    }
    else {
        ESP_LOGE(TAG, "Could not update sensor");
    }
}

}  // namespace rego
}  // namespace esphome
