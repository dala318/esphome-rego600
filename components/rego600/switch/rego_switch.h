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
    void dump_config() override ;
    void write_state(bool state) override;
    void set_retry_write(uint8_t retry) { this->max_retry_attempts_ = retry; }
protected:
    uint8_t max_retry_attempts_ = 0;
    uint8_t attempt_ = 0;
    bool retry_value_;
};

}  // namespace rego
}  // namespace esphome
