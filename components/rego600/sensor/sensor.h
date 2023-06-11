#include "esphome.h"
#include "esphome/components/rego600/rego_interface.h"

#include <vector>

namespace esphome {
namespace rego {

class RegoSensor : public sensor::Sensor, public RegoBase {
public:
    // void setup() override { this->publish_state(50); }
    // void update() override {
    //     // auto ip = wifi::global_wifi_component->wifi_sta_ip();
    //     // if (ip != this->last_ip_) {
    //     //   this->last_ip_ = ip;
    //     //   this->publish_state(ip.str());
    //     // }
    //     float value = 5050.8;
    //     if (value != this->last_value_) {
    //         this->last_value_ = value;
    //         // this->publish_state(value);
    //     }
    // }
//   std::string unique_id() override { return get_mac_address() + "-wifiinfo-ip"; }
//   void dump_config() override;
protected:
    float last_value_;
//   network::IPAddress last_ip_;
};

}  // namespace rego
}  // namespace esphome
