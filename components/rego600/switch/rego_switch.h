#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoSwitch: public switch_::Switch, public RegoBase {
public:
    void setup() override;
    void loop() override;
    void dump_config() override;
    void update() override;
    void write_state(bool state) override;
    void set_retry_write(uint8_t retry) { this->max_retry_attempts_ = retry; }
    void set_action_payload_true(uint8_t payload) { this->action_payload_true_ = payload; }
    void set_action_payload_false(uint8_t payload) { this->action_payload_false_ = payload; }
protected:
    uint8_t max_retry_attempts_ = 0;
    int16_t action_payload_true_ = 1;
    int16_t action_payload_false_ = 0;
    uint8_t attempt_ = 0;
    bool retry_value_;
};

}  // namespace rego
}  // namespace esphome
