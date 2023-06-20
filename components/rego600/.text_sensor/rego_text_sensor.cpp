#include "rego_text_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.text_sensor";

void RegoTextSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego TextSensor:");
    LOG_TEXT_SENSOR("  ", "TextSensor", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int2hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

}  // namespace rego
}  // namespace esphome
