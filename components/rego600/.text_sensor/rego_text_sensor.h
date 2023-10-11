#pragma once

#include "esphome.h"
#include "esphome/components/rego600/rego.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoTextSensor : public text_sensor::TextSensor, public RegoBase {
public:
    void dump_config() override;
    void update() override;
protected:
};

}  // namespace rego
}  // namespace esphome
