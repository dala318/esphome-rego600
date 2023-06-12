#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego_interface.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoTextSensor : public text_sensor::TextSensor, public RegoBase {
public:
    void dump_config() override;
protected:
    std::string last_value_;
};

}  // namespace rego
}  // namespace esphome
