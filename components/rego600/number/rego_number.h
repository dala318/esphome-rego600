#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoNumber : public number::Number, public RegoBase {
public:
    void dump_config() override;
    void control(float value) override { this->publish_state(value); }
    void set_value_factor(float value_factor) { this->value_factor_ = value_factor; }
protected:
    float last_value_;
    float value_factor_;
};

}  // namespace rego
}  // namespace esphome
