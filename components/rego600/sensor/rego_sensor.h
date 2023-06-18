#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoSensor : public sensor::Sensor, public RegoBase {
public:
    // void setup() override { this->publish_state(50); }
    void dump_config() override;
    void update() override;

    void set_value_factor(float value_factor) { this->value_factor_ = value_factor; }
protected:
    float value_factor_;
};

}  // namespace rego
}  // namespace esphome
