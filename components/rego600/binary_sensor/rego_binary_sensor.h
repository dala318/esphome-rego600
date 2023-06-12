#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego_interface.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoBinarySensor : public binary_sensor::BinarySensor, public RegoBase {
public:
    void dump_config() override;
protected:
    bool last_value_;
};

}  // namespace rego
}  // namespace esphome
