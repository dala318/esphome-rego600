#include "rego_text_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.text_sensor";

void RegoTextSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego TextSensor:");
    LOG_TEXT_SENSOR("  ", "TextSensor", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int_to_hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

void RegoTextSensor::update() {
    std::string result;
    if (this->hub_->read_text(this->rego_variable_, &result)) {
        this->publish_state(result);
    }
    else {
        ESP_LOGE(TAG, "Could not update text_sensor \"%s\"", this->get_name().c_str());
    }
}

}  // namespace rego
}  // namespace esphome
