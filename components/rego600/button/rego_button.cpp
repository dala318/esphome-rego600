#include "rego_button.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.button";

void RegoButton::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Button:");
    LOG_BUTTON("  ", "Button", this);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

void RegoButton::press_action() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) { }
    else {
        ESP_LOGE(TAG, "Could not perform button command");
    }
}

}  // namespace rego
}  // namespace esphome
