#include "rego_number.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.number";

void RegoNumber::setup() {
    ESP_LOGD(TAG, "Restoring number %s", this->get_name().c_str());
    int16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->publish_state(result * this->value_factor_);
    }
}

void RegoNumber::loop() {
    if ((this->attempt_ != 0) && !this->hub_->get_uart_bussy()) {
        if (this->attempt_ <= this->max_retry_attempts_) {
            this->control(this->retry_value_);
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

void RegoNumber::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Number:");
    LOG_NUMBER("  ", "Number", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int_to_hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Value factor: %f", this->value_factor_);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

void RegoNumber::update() {
    int16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->publish_state(result * this->value_factor_);
    }
    else {
        ESP_LOGE(TAG, "Could not update number \"%s\"", this->get_name().c_str());
    }
}

void RegoNumber::control(float value) {
    uint16_t result = 0;
    if (this->hub_->write_value(this->rego_variable_, (int16_t)(value / this->value_factor_), &result)) {
        this->publish_state(value);
        this->attempt_ = 0;
    }
    else {
        ESP_LOGE(TAG, "Could not write \"%f\" to number \"%s\"", value, this->get_name().c_str());
        if ((this->max_retry_attempts_ != 0) && (this->attempt_ == 0)) {
            this->attempt_ = 1;
            this->retry_value_ = value;
            if (this->hub_->get_log_all()) {
                ESP_LOGI(TAG, "Scheduling for retry");
            }
        }
    }
}

}  // namespace rego
}  // namespace esphome
