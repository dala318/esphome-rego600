#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoClimate : public climate::Climate, public RegoBase {
public:
    RegoClimate() {
        this->preset = climate::CLIMATE_PRESET_HOME;
        this->mode = climate::CLIMATE_MODE_HEAT;
        this->action = climate::CLIMATE_ACTION_HEATING;
    }

    climate::ClimateTraits traits() override {
        // The capabilities of the climate device
        auto traits = climate::ClimateTraits();
        traits.set_supports_current_temperature(true);
        traits.set_supported_modes({climate::CLIMATE_MODE_HEAT});
        traits.set_visual_min_temperature(12.0);
        traits.set_visual_max_temperature(25.0);
        traits.set_visual_temperature_step(0.1);
        return traits;
    }

    // virtual void publish(float value) override {
    virtual void publish(float value) {
        this->target_temperature = value / this->value_factor_;
        this->publish_state();
    }

    void control(const climate::ClimateCall &call) override {
        if (call.get_target_temperature().has_value()) {
            this->target_temperature = *call.get_target_temperature();
            this->publish_state();
            int32_t indoor_setpoint = this->target_temperature * this->value_factor_;
            // this->send_data(this->can_poll_id, indoor_setpoint);
        }
    }



    void dump_config() override;
    void set_value_factor(float value_factor) { this->value_factor_ = value_factor; }
    void set_indoor_sensor(sensor::Sensor *indoor_sensor) {
        this->indoor_sensor_ = indoor_sensor;
        // this->set_interval("update_indoor_temperature", 5000, [this]() { this->update_indoor_temperature(); });
    }

    void update_indoor_temperature() {
        if (this->indoor_sensor_->has_state()) {
            this->current_temperature = this->indoor_sensor_->state;
            this->publish_state();
            int32_t indoor_temp = this->current_temperature * 10;
            // this->send_data(INDOOR_THERMOSTAT_DIAL_CAN_ID, INDOOR_THERMOSTAT_DIAL_MIDPOINT);
            // this->send_data(INDOOR_THERMOSTAT_TEMP_CAN_ID, indoor_temp);
        }
    }
protected:
    float value_factor_;
    sensor::Sensor *indoor_sensor_;};

}  // namespace rego
}  // namespace esphome
