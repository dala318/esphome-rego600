#include "rego_text_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "Rego.TextSensor";

void RegoTextSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego TextSensor:");
    LOG_TEXT_SENSOR("  ", "TextSensor", this);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

}  // namespace rego
}  // namespace esphome
