#include "rego_number.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.number";

void RegoNumber::setup() {
    ESP_LOGD(TAG, "Restoring number %s", this->get_name().c_str());
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->publish_state(result);
    }
}

void RegoNumber::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Number:");
    LOG_NUMBER("  ", "Number", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int2hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Value factor: %f", this->value_factor_);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

void RegoNumber::control(float value) {
    this->publish_state(value);
    
    uint16_t result = 0;
    if (this->hub_->write_value(this->rego_variable_, value, &result)) { }  // TODO: Ensure command is sent if UART bussy, retry or queue in hub?
    else {
        ESP_LOGE(TAG, "Could not write %u to number %s", value, this->get_name().c_str());
    }
}

}  // namespace rego
}  // namespace esphome
