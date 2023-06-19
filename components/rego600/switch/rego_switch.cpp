#include "rego_switch.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.switch";

void RegoSwitch::setup() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->publish_state(result != 0);
    }
}

void RegoSwitch::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Switch:");
    LOG_SWITCH("  ", "Switch", this);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

void RegoSwitch::write_state(bool state) {
    uint16_t result = 0;
    uint16_t value = (uint16_t)state;
    if (this->hub_->write_value(this->rego_variable_, value, &result)) {
        this->publish_state(result != 0);
    }
    else {
        ESP_LOGE(TAG, "Could not set %s switch %s", (state ? "true" : "false"), this->get_name().c_str());
    }
}

}  // namespace rego
}  // namespace esphome
