#include "rego_binary_sensor.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.binary_sensor";

void RegoBinarySensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego BinarySensor:");
    LOG_BINARY_SENSOR("  ", "BinarySensor", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int_to_hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Payload true: %s", this->int_to_hex(this->payload_true_).c_str());
    // ESP_LOGCONFIG(TAG, "  Payload false: %s", this->int_to_hex(this->action_payload_false).c_str());
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

void RegoBinarySensor::update() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        // this->publish_state(result != 0);
        this->publish_state(result == this->payload_true_);
    }
    else {
        ESP_LOGE(TAG, "Could not update binary sensor \"%s\"", this->get_name().c_str());
    }
}

}  // namespace rego
}  // namespace esphome
