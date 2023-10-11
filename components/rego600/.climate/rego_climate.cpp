#include "rego_climate.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego.climate";

void RegoClimate::setup() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->target_temperature = result * this->value_factor_;
        this->publish_state();
    }
}

void RegoClimate::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Climate:");
    LOG_CLIMATE("  ", "Climate", this);
    ESP_LOGCONFIG(TAG, "  Rego variable: 0x%s", this->int_to_hex(this->rego_variable_).c_str());
    ESP_LOGCONFIG(TAG, "  Value factor: %f", this->value_factor_);
    ESP_LOGCONFIG(TAG, "  Hub: %s", this->hub_->to_str().c_str());
}

void RegoClimate::update() {
    uint16_t result = 0;
    if (this->hub_->read_value(this->rego_variable_, &result)) {
        this->target_temperature = result * this->value_factor_;
        this->publish_state();
    }
    else {
        ESP_LOGE(TAG, "Could not update climate \"%s\"", this->get_name().c_str());
    }
}

void RegoClimate::control(const climate::ClimateCall &call) {
    if (call.get_target_temperature().has_value()) {
        float target_temperature = *call.get_target_temperature();
        uint16_t result = 0;
        if (this->hub_->write_value(this->rego_variable_, (uint16_t)(target_temperature / this->value_factor_), &result)) {
            this->target_temperature = target_temperature;
            this->publish_state();
        }
    }
}

void RegoClimate::update_indoor_temperature() {
    if (this->indoor_sensor_->has_state()) {
        if (this->current_temperature != this->indoor_sensor_->state) {
            this->current_temperature = this->indoor_sensor_->state;
            this->publish_state();
        }
        int32_t indoor_temp = this->current_temperature / this->value_factor_;
        // this->hub_->write_value(INDOOR_THERMOSTAT_DIAL_CAN_ID, INDOOR_THERMOSTAT_DIAL_MIDPOINT);
        // this->hub_->write_value(INDOOR_THERMOSTAT_TEMP_CAN_ID, indoor_temp);
    }
}

}  // namespace rego
}  // namespace esphome
