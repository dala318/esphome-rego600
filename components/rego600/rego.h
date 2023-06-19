/*!
 * Base definition for regi_interface with class definitions for both internal and external use.
 * 
 */

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#include "constants.h"

#include <cstdint>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace esphome {
namespace rego {

class RegoInterfaceComponent : public Component {
public:
    // Function override declarations
    void setup() override { this->bussy_ = false; }
    void dump_config() override;
    void loop() override;

    // Function override definitions

    // Function declaration component
    bool read_value(int16_t reg, uint16_t *result);
    bool write_value(int16_t reg, uint16_t value, uint16_t *result);

    // Function definitions component
    void set_model(std::string model){ this->model_ = model; }
    void set_log_all(bool log_all) {this->log_all_ = log_all; }
    void set_retry_sleep(int sleep) {this->read_retry_sleep_ = sleep; }
    void set_retry_attempts(uint8_t attempts) {this->read_retry_attempts_ = attempts; }
    void set_uart_parent(esphome::uart::UARTComponent *parent) { this->uart_ = parent; }
    std::string to_str() {
        return "Model: " + this->model_ + " UART: " + std::to_string(this->uart_->get_baud_rate()) + " baud";
    }

protected:
    // Function definitions
    bool command_and_response(uint8_t addr, uint8_t cmd, uint16_t reg, uint16_t val, uint16_t *result);
    void int2write (int16_t value, uint8_t *write_array); // convert int (16 bit) to array for sending
    int16_t read2int(uint8_t *read_array);
    std::string data2hexstr(const uint8_t *data, size_t len);

    // Thread mutex
    bool bussy_ = false;
    uint16_t bussy_counter_ = 0;

    // Config parameters
    esphome::uart::UARTComponent *uart_{nullptr};
    std::string model_;
    bool log_all_;
    uint8_t read_retry_attempts_ = 5;
    uint8_t read_retry_sleep_ = 10;
};

class RegoBase : public PollingComponent {
public:
    // virtual void setup() override {
    //     this->setup_listening();
    //     if (this->is_polling) {
    //         this->setup_polling();
    //     }
    // }

    void register_hub(RegoInterfaceComponent *hub){ this->hub_ = hub; }

    void set_rego_variable(std::uint16_t rego_variable) { this->rego_variable_ = rego_variable; }

    void update() override { }

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
    std::string int2hex(std::uint16_t value) {
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(sizeof(value)*2) << std::hex << value;
        return stream.str();
    }

    RegoInterfaceComponent *hub_;
    std::uint16_t rego_variable_;
};


// // Parameter register class
// class Parameter {
// public:
//     Parameter(std::int64_t reg_, std::string ident_) {
//         reg = reg_;
//         ident = ident_;
//     }

//     std::string identifier(void) {
//         return ident;
//     }

//     void update_values(int new_value) {
//         b_value = new_value > 100;
//         f_value = static_cast<float>(new_value);
//         t_value = std::to_string(new_value);

//         publish_entities();
//     }

// private:
//     // Function declararations
//     void publish_entities();

//     // Values
//     bool b_value;
//     float f_value;
//     std::string t_value;

//     // Identifiers
//     std::int64_t reg;
//     std::string ident;
// };

}  // namespace rego
}  // namespace esphome
