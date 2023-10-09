#include "rego_switch.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.switch";

void RegoSwitch::setup() {
    ESP_LOGD(TAG, "Restoring switch %s", this->get_name().c_str());
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->publish_state(result != 0);
    }
}

void RegoSwitch::loop() {
    if ((this->attempt_ != 0) && !this->hub_->get_uart_bussy()) {
        if (this->attempt_ <= this->max_retry_attempts_) {
            this->write_state(this->retry_value_);
            this->attempt_++;
        }
        else {
            if (this->hub_->get_log_all()) {
                ESP_LOGI(TAG, "Abort retry after %u attempts", this->attempt_);
            }
            this->attempt_ = 0;
        }
    }
}

void RegoSwitch::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Switch:");
    LOG_SWITCH("  ", "Switch", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int2hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);
}

void RegoSwitch::write_state(bool state) {
    uint16_t result = 0;
    uint16_t value = (uint16_t)state;
    if (this->hub_->write_value(this->rego_variable_, value, &result)) {
        this->publish_state(result != 0);
        this->attempt_ = 0;
    }
    else {
        ESP_LOGE(TAG, "Could not set \"%s\" to switch \"%s\"", (state ? "true" : "false"), this->get_name().c_str());
        if ((this->max_retry_attempts_ != 0) && (this->attempt_ == 0)) {
            this->attempt_ = 1;
            this->retry_value_ = state;
            if (this->hub_->get_log_all()) {
                ESP_LOGI(TAG, "Scheduling for retry");
            }
        }
    }
}

}  // namespace rego
}  // namespace esphome
