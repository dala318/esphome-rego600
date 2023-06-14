#include "rego_button.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.button";

void RegoButton::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Button:");
    LOG_BUTTON("  ", "Button", this);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

}  // namespace rego
}  // namespace esphome
