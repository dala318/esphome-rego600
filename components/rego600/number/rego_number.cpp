#include "rego_number.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.number";

void RegoNumber::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Number:");
    LOG_NUMBER("  ", "Number", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int2hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Value factor: %f", this->value_factor_);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

}  // namespace rego
}  // namespace esphome
