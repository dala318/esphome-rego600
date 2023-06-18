#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoButton: public button::Button, public RegoBase {
public:
    // void setup() override;
    void press_action() override;
    void dump_config() override;
protected:
};

}  // namespace rego
}  // namespace esphome
