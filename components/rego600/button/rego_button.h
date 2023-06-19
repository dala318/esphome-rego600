#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoButton: public button::Button, public RegoBase {
public:
    void loop() override;
    void press_action() override;
    void dump_config() override;
    void set_retry_write(uint8_t retry) { this->max_retry_attempts_ = retry; }
protected:
    uint8_t max_retry_attempts_ = 0;
    uint8_t attempt_ = 0;
};

}  // namespace rego
}  // namespace esphome
