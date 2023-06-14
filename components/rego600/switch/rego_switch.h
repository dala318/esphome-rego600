#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoSwitch: public switch_::Switch, public RegoBase {
public:
    // void setup() override;
    void write_state(bool state) override { this->publish_state(state); }
    void dump_config() override ;
protected:
};

}  // namespace rego
}  // namespace esphome
