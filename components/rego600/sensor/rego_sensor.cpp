#include "rego_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "Rego.Sensor";

void RegoSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Sensor:");
    LOG_SENSOR("  ", "Sensor", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: %u", this->rego_variable_);
    ESP_LOGCONFIG(TAG, "  Value factor: %f", this->value_factor_);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

}  // namespace rego
}  // namespace esphome
