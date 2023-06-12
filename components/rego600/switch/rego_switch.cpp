#include "rego_switch.h"

namespace esphome {
namespace rego {

static const char *TAG = "Rego.Switch";

void RegoSwitch::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Switch:");
    LOG_SWITCH("  ", "Switch", this);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

}  // namespace rego
}  // namespace esphome
