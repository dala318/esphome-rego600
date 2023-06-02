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

static const std::int16_t registers = 0x0000;
/*
600-635      636-         Register */ /*
//Sensor values
0x0209         0x020B         "Radiator return [GT1]"
0x020A         0x020C         "Outdoor [GT2]"
0x020B         0x020D         "Hot water [GT3]"
0x020C         0x020E         "Forward [GT4]"
0x020D         0x020F         "Room [GT5]"
0x020E         0x0210         "Compressor [GT6]"
0x020F         0x0211         "Heat fluid out [GT8]"
0x0210         0x0212         "Heat fluid in [GT9]"
0x0211         0x0213         "Cold fluid in [GT10]"
0x0212         0x0214         "Cold fluid out [GT11]"
0x0213         0x0215         "External hot water [GT3x]"
//Device values
0x01FD         0x01FF         "Ground loop pump [P3]"
0x01FE         0x0200         "Compresor"
0x01FF         0x0201         "Additional heat 3kW"
0x0200         0x0202         "Additional heat 6kW"
0x0203         0x0205         "Radiator pump [P1]"
0x0204         0x0206         "Heat carrier pump [P2]"
0x0205         0x0207         "Tree-way valve [VXV]"
0x0206         0x0208         "Alarm"
//Control data
0x006E         0x006E         "GT1 Target value"
0x006F         0x006F         "GT1 On value"
0x0070         0x0070         "GT1 Off value"
0x002B         0x002B         "GT3 Target value"
0x0073         0x0073         "GT3 On value"
0x0074         0x0074         "GT3 Off value"
0x006D         0x006D         "GT4 Target value"
0x006C         0x006C         "Add heat power in %"
//Settings
0x0000         0x0000         "Heat curve"
0x0001         0x0001         "Heat curve fine adj."
0x0021         0x0021         "Indoor temp setting"
0x0022         0x0022         "Curve infl. by in-temp."
0x001E         0x001E         "Adj. curve at 20° out"
0x001C         0x001C         "Adj. curve at 15° out"
0x001A         0x001A         "Adj. curve at 10° out"
0x0018         0x0018         "Adj. curve at 5° out"
0x0016         0x0016         "Adj. curve at 0° out"
0x0014         0x0014         "Adj. curve at -5° out"
0x0012         0x0012         "Adj. curve at -10° out"
0x0010         0x0010         "Adj. curve at -15° out"
0x000E         0x000E         "Adj. curve at -20° out"
0x000C         0x000C         "Adj. curve at -25° out"
0x000A         0x000A         "Adj. curve at -30° out"
0x0008         0x0008         "Adj. curve at -35° out"
0x0002         0x0002         "Heat curve coupling diff."
*/

// Binary Sensor specific classes
class BinarySensorConnected : public binary_sensor::BinarySensor, public Component {
public:
protected:
    bool last_value_;
};
class BinarySensorHeatNeeded : public binary_sensor::BinarySensor, public Component {
public:
protected:
    bool last_value_;
};

// Sensor specific classes
class SensorPower : public sensor::Sensor, public Component {
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
class SensorReturnTemp : public sensor::Sensor, public Component {
public:
protected:
    float last_value_;
};

// Text Sensor specific classes
class TextSensorDeviceType : public text_sensor::TextSensor, public Component {
public:
protected:
    std::string last_value_;
};
class TextSensorDeviceModel : public text_sensor::TextSensor, public Component {
public:
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

// Parameter register class
class Parameter {
public:
    Parameter(std::int64_t reg_, std::string ident_) {
        reg = reg_;
        ident = ident_;
    }

    std::string identifier(void) {
        return ident;
    }

    void update_values(int new_value) {
        b_value = new_value > 100;
        f_value = static_cast<float>(new_value);
        t_value = std::to_string(new_value);

        publish_entities();
    }

    // Subscribers
    std::vector<binary_sensor::BinarySensor *> binary_sensors_;
    std::vector<sensor::Sensor *> sensors_;
    std::vector<text_sensor::TextSensor *> text_sensors_;

private:
    // Function declararations
    void publish_entities();

    // Values
    bool b_value;
    float f_value;
    std::string t_value;

    // Identifiers
    std::int64_t reg;
    std::string ident;
};

class H60InterfaceComponent : public Component {
public:
    // Function override declarations
    void setup() override;
    void loop() override;
    void dump_config() override;
    void on_shutdown() override;

    // Function override definitions
    float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

    // Function declaration component
    void set_model(std::string model);

    // Function definitions component
    void set_uart_parent(esphome::uart::UARTComponent *parent) { this->stream_ = parent; }
    void set_buffer_size(size_t size) { this->buf_size_ = size; }
    
    // Function declarations register input entities
    void register_binary_sensor(std::string id, binary_sensor::BinarySensor *obj);
    void register_sensor(std::string id, sensor::Sensor *obj);
    void register_text_sensor(std::string id, text_sensor::TextSensor *obj);

    // Function definitions register output entities
    void register_switch(std::string id, switch_::Switch *obj) { this->switches_.push_back(obj); /* TODO, use id in-paramter to link the entity to a parameter to the heat-pump and register a callback to update on "sensor->write_state(bool);" */}

protected:
    // Function definitions
    void accept();
    void read();
    void flush();
    void write();
    void cleanup();

    // Registred input entities
    std::vector<binary_sensor::BinarySensor *> binary_sensors_; // TODO: remove
    std::vector<sensor::Sensor *> sensors_; // TODO: remove
    std::vector<text_sensor::TextSensor *> text_sensors_; // TODO: remove

    // Registred ouputut entities
    std::vector<switch_::Switch *> switches_;

    // For testing purposes only
    int loop_counter = 0;

    // Internal state storage
    std::vector<Parameter *> parameters_{};

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