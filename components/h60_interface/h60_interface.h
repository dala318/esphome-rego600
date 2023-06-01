#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

// Input entities
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

// Output entities
#include "esphome/components/switch/switch.h"

// #include "HD_Globals.h"
// #include "HD_Utilities.h"
// #include "HD_H1comm.h"

#include <memory>
#include <string>
#include <vector>

namespace esphome {
namespace h60_interface {

// Binary Sensor specific classes
class BinarySensorConnected : public binary_sensor::BinarySensor, public PollingComponent {
public:
    void update() override {
        bool value = true;
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
  }
protected:
    bool last_value_;
};
class BinarySensorHeatNeeded : public binary_sensor::BinarySensor, public PollingComponent {
public:
    void update() override {
        bool value = false;
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
    }
protected:
    bool last_value_;
};

// Sensor specific classes
class SensorPower : public sensor::Sensor, public PollingComponent {
public:
    // void setup() override { this->publish_state(50); }
    void update() override {
        // auto ip = wifi::global_wifi_component->wifi_sta_ip();
        // if (ip != this->last_ip_) {
        //   this->last_ip_ = ip;
        //   this->publish_state(ip.str());
        // }
        float value = 5050.8;
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
  }
//   std::string unique_id() override { return get_mac_address() + "-wifiinfo-ip"; }
//   void dump_config() override;
protected:
    float last_value_;
//   network::IPAddress last_ip_;
};
class SensorReturnTemp : public sensor::Sensor, public PollingComponent {
public:
    void update() override {
        float value = -701.888;
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
    }
protected:
    float last_value_;
};

// Text Sensor specific classes
class TextSensorDeviceType : public text_sensor::TextSensor, public PollingComponent {
public:
    void update() override {
        std::string value = "rego 600";
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
  }
protected:
    std::string last_value_;
};
class TextSensorDeviceModel : public text_sensor::TextSensor, public PollingComponent {
public:
    void update() override {
        std::string value = "3.1";
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
    }
protected:
    std::string last_value_;
};

// Switch specific classes
class SwitchAdditionalHeat: public switch_::Switch, public Component {
public:
    // void setup() override;
    void write_state(bool state) override { this->publish_state(state); }
    // void dump_config() override;
protected:
    // bool last_state_;
};


class H60InterfaceComponent : public Component {
public:
    void setup() override;
    void loop() override;
    void dump_config() override;
    void on_shutdown() override;

    void set_model(std::string model) {this->model_ = model; }
    void set_uart_parent(esphome::uart::UARTComponent *parent) { this->stream_ = parent; }
    void set_buffer_size(size_t size) { this->buf_size_ = size; }
    
    float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

    // Register input entities function
    void register_binary_sensor(std::string id, binary_sensor::BinarySensor *obj) { this->binary_sensors_.push_back(obj); }
    void register_sensor(std::string id, sensor::Sensor *obj) { this->sensors_.push_back(obj); /* TODO, use id in-paramter to link the entity to a parameter from the heat-pump and register a callback in that parameter to set the value "sensor->publish_state(value);" */}
    void register_text_sensor(std::string id, text_sensor::TextSensor *obj) { this->text_sensors_.push_back(obj); }

    // Register output entities function
    void register_switch(std::string id, switch_::Switch *obj) { this->switches_.push_back(obj); /* TODO, use id in-paramter to link the entity to a parameter to the heat-pump and register a callback to update on "sensor->write_state(bool);" */}

    // float get_float_parameter(std::string parameter) {
    //     if (paramter == 0){
    //         return 550.88
    //     } else if (paramter == 1) {
    //         return -188.22222
    //     }
    //     return 0
    // }

protected:
    void accept();
    void cleanup();
    void read();
    void flush();
    void write();

    // Registred input entities
    std::vector<binary_sensor::BinarySensor *> binary_sensors_;
    std::vector<sensor::Sensor *> sensors_;
    std::vector<text_sensor::TextSensor *> text_sensors_;

    // Registred ouputut entities
    std::vector<switch_::Switch *> switches_;

    // All available heat-pump parameters
    // typedef enum
    // {
    //     BINARY_SENSOR
    //     SENSOR,
    //     TEXT_SENSOR,
    // } entity_type;
    // struct Parameter {
    //     Parameter(std::int64_t reg, std::string identifier, size_t position);

    //     std::int64_t reg{};
    //     std::string identifier{};
    //     // bool disconnected{false};
    //     size_t position{0};
    // };
    // std::vector<Parameter> parameters_{};

    // Keeping track of UART bus data
    size_t buf_index(size_t pos) { return pos & (this->buf_size_ - 1); }
    size_t buf_ahead(size_t pos) { return (pos | (this->buf_size_ - 1)) - pos + 1; }

    std::string model_;
    esphome::uart::UARTComponent *stream_{nullptr};
    size_t buf_size_;

    std::unique_ptr<uint8_t[]> buf_{};
    size_t buf_head_{0};
    size_t buf_tail_{0};
};

}  // namespace h60_interface
}  // namespace esphome