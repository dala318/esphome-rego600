#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoBinarySensor : public binary_sensor::BinarySensor, public RegoBase {
public:
    void dump_config() override;
    void update() override;
    void set_payload_true(uint8_t payload) { this->payload_true_ = payload; }
    // void set_action_payload_false(uint8_t payload) { this->action_payload_false_ = payload; }
protected:
    uint16_t payload_true_ = 1;
    // uint16_t action_payload_false = 0;
};

}  // namespace rego
}  // namespace esphome
