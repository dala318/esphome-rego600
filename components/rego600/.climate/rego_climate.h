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

    void setup() override;
    void dump_config() override;
    void update() override;
    void control(const climate::ClimateCall &call) override;
    void set_value_factor(float value_factor) { this->value_factor_ = value_factor; }
    void update_indoor_temperature();
    void set_indoor_sensor(sensor::Sensor *indoor_sensor) {
        this->indoor_sensor_ = indoor_sensor;
        this->set_interval("update_indoor_temperature", 5000, [this]() { this->update_indoor_temperature(); });
    }


protected:
    float value_factor_;
    sensor::Sensor *indoor_sensor_;};

}  // namespace rego
}  // namespace esphome
