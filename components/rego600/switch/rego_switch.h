#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoSwitch: public switch_::Switch, public RegoBase {
public:
    void setup() override;
    void dump_config() override ;
    void write_state(bool state) override;
protected:
};

}  // namespace rego
}  // namespace esphome
