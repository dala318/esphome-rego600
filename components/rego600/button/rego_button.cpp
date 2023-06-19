#include "rego_button.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.button";

void RegoButton::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Button:");
    LOG_BUTTON("  ", "Button", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int2hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

void RegoButton::press_action() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) { }  // TODO: Ensure command is sent if UART bussy, retry or queue in hub?
    else {
        ESP_LOGE(TAG, "Could not perform button command");
    }
}

}  // namespace rego
}  // namespace esphome
