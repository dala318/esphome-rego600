/*!
 * Base definition for regi_interface with class definitions for both internal and external use.
 * 
 */

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

// // Input entities
// #include "esphome/components/binary_sensor/binary_sensor.h"
// #include "esphome/components/sensor/sensor.h"
// #include "esphome/components/text_sensor/text_sensor.h"

// // Output entities
// #include "esphome/components/button/button.h"
// #include "esphome/components/switch/switch.h"

// #include "HD_Globals.h"
// #include "HD_Utilities.h"
// #include "HD_H1comm.h"
#include "constants.h"

#include <cstdint>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace esphome {
namespace rego {


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
    // std::vector<RegoBinarySensor *> binary_sensors_;
    // std::vector<RegoSensor *> sensors_;
    // std::vector<RegoTextSensor *> text_sensors_;

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

class RegoInterfaceComponent : public Component {
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
    void set_log_all(bool log_all) {this->log_all_ = log_all; }
    // void set_update_interval(int update_interval) { this->update_interval_ = update_interval; }
    void set_uart_parent(esphome::uart::UARTComponent *parent) { this->stream_ = parent; }
    void set_buffer_size(size_t size) { this->buf_size_ = size; }
    
    // Function declarations register input entities
    // void register_binary_sensor(std::string id, RegoBinarySensor *obj);
    // void register_sensor(std::string id, RegoSensor *obj);
    // void register_text_sensor(std::string id, RegoTextSensor *obj);

    // Function definitions register output entities
    // void register_switch(std::string id, RegoSwitch *obj) {
        // this->switches_.push_back(obj);
        // TODO, use id in-paramter to link the entity to a parameter to the heat-pump and register a callback to update on "sensor->write_state(bool);"
    // }

protected:
    // Function definitions
    void write_registers();
    void read_registers();
    std::string  read_value(int16_t reg, std::string name);
    // command_and_response(std::byte addr, std::byte cmd, int16_t reg, int16_t val);

    // Registred input entities
    // std::vector<RegoBinarySensor *> binary_sensors_; // TODO: remove
    // std::vector<RegoSensor *> sensors_; // TODO: remove
    // std::vector<RegoTextSensor *> text_sensors_; // TODO: remove

    // Registred ouputut entities
    // std::vector<RegoSwitch *> switches_;

    // For testing purposes only
    int loop_counter = 0;
    std::int16_t reg = registers;

    // Internal state storage
    std::vector<Parameter *> parameters_{};
    std::map<std::string, Parameter *> parameters_map_{};
    int last_update_ = 0;

    // Config parameters
    std::string model_;
    bool log_all_ = false;
    // int update_interval_ = 2000;
    size_t buf_size_;
    esphome::uart::UARTComponent *stream_{nullptr};

    // Keeping track of UART bus data
    size_t buf_index(size_t pos) { return pos & (this->buf_size_ - 1); }
    size_t buf_ahead(size_t pos) { return (pos | (this->buf_size_ - 1)) - pos + 1; }

    std::unique_ptr<uint8_t[]> buf_{};
    size_t buf_head_{0};
    size_t buf_tail_{0};
};

class RegoBase : public PollingComponent {
public:
    void register_hub(RegoInterfaceComponent *hub);

    // virtual void setup() override {
    //     this->setup_listening();
    //     if (this->is_polling) {
    //         this->setup_polling();
    //     }
    // }

    void update() override { int a = 1; }

    int parse_data(std::vector<uint8_t> data) {
        // if (data.size() == 1) {
        //     return (int8_t)data[0];
        // } else if (data.size() == 2) {
        //     return (int16_t)(data[0] << 8 | data[1]);
        // } else if (data.size() == 4) {
        //     return (int32_t)(data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]);
        // } else {
        //     ESP_LOGE(TAG, "Received DLC %d is invalid. Has to be 1, 2 or 4", data.size());
        // }
        return 0;
    }

    // /* CAN listening */
    // void setup_listening() {
    //     this->can_trigger = new CanbusTriggerProxy(this->canbus, this->can_recv_id, this);
    //     this->can_trigger->setup();
    // }

    // virtual void data_recv(std::vector<uint8_t> data, uint32_t can_id) {
    //     this->inferred_data_len = data.size();
    //     int value = this->parse_data(data);
    //     this->publish(value * this->value_factor);
    // }
    // virtual void publish(float value)=0;

    // /* CAN polling */
    // void setup_polling() {
    //     this->set_interval("rego_poll", this->poll_interval, [this]() { this->rego_poll(); });
    // }
    // void rego_poll() {
    //     this->canbus->send_data(this->can_poll_id, true, true, std::vector<uint8_t>());
    // }

    // /* Config */
    // void set_canbus(canbus::Canbus *canbus) {
    //     this->canbus = canbus;
    // }

    // void set_rego_variable(uint16_t rego_variable) {
    //     this->can_recv_id = 0x0C003FE0 | (rego_variable << 14);
    //     this->can_poll_id = (rego_variable << 14) | 0x04003FE0;
    //     this->is_polling = true;
    // }

    // void set_rego_listen_can_id(uint32_t can_id) {
    //     this->can_recv_id = can_id;
    //     this->can_poll_id = 0;
    //     this->is_polling = false;
    // }

    // void set_value_factor(float value_factor) {
    //     this->value_factor = value_factor;
    // }

    // void set_poll_interval(uint32_t poll_interval) {
    //     this->poll_interval = poll_interval;
    // }

    // void send_data(uint32_t can_id, int32_t value) {
    //     if (this->inferred_data_len == 1) {
    //     std::vector<uint8_t> can_data = std::vector<uint8_t>({ (uint8_t)value });
    //     this->canbus->send_data(can_id, true, false, can_data);
    //     } else if (this->inferred_data_len == 2) {
    //     std::vector<uint8_t> can_data = std::vector<uint8_t>({ (uint8_t)((value >> 8) & 0xff), (uint8_t)(value & 0xff) });
    //     this->canbus->send_data(can_id, true, false, can_data);
    //     } else if (this->inferred_data_len == 4) {
    //     std::vector<uint8_t> can_data = std::vector<uint8_t>({ (uint8_t)((value >> 24) & 0xff), (uint8_t)((value >> 16) & 0xff), (uint8_t)((value >> 8) & 0xff), (uint8_t)(value & 0xff) });
    //     this->canbus->send_data(can_id, true, false, can_data);
    //     } else {
    //     ESP_LOGE(TAG, "Tried to send data with invalid inferred length %d", this->inferred_data_len);
    //     }
    // }

protected:
    RegoInterfaceComponent *hub_;
    // uint32_t can_recv_id;
    // uint32_t can_poll_id;
    // uint32_t poll_interval;
    // uint8_t inferred_data_len = 0;
    // bool is_polling = false;
    // float value_factor = 1;
};

}  // namespace rego
}  // namespace esphome
