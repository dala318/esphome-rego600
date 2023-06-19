#include "rego_button.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.button";

void RegoButton::loop() {
    if ((this->attempt_ != 0) && !this->hub_->get_uart_bussy()) {
        if (this->attempt_ <= this->max_retry_attempts_) {
            this->press_action();
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

void RegoButton::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Button:");
    LOG_BUTTON("  ", "Button", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int2hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_);  // TODO: Need a "to_str" representation
}

void RegoButton::press_action() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->attempt_ = 0;
    }
    else {
        ESP_LOGE(TAG, "Could not perform button command %s", this->get_name().c_str());
        if ((this->max_retry_attempts_ != 0) && (this->attempt_ == 0)) {
            this->attempt_ = 1;
            if (this->hub_->get_log_all()) {
                ESP_LOGI(TAG, "Scheduling for retry");
            }
        }
    }
}

}  // namespace rego
}  // namespace esphome
