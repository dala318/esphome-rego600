#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoNumber : public number::Number, public RegoBase {
public:
    void setup() override;
    void loop() override;
    void dump_config() override;
    void control(float value) override;
    void set_value_factor(float value_factor) { this->value_factor_ = value_factor; }
    void set_retry_write(uint8_t retry) { this->max_retry_attempts_ = retry; }
protected:
    float value_factor_;
    uint8_t max_retry_attempts_ = 0;
    uint8_t attempt_ = 0;
    float retry_value_;
};

}  // namespace rego
}  // namespace esphome
