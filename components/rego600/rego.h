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

#define MAX_READ 128

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
    void set_read_delay(int delay) {this->read_delay_ = delay; }
    void set_retry_sleep(int sleep) {this->read_retry_sleep_ = sleep; }
    void set_retry_attempts(uint8_t attempts) {this->read_retry_attempts_ = attempts; }
    void set_uart_parent(esphome::uart::UARTComponent *parent) { this->uart_ = parent; }
    bool get_uart_bussy() { return this->bussy_; }
    bool get_log_all() { return this->log_all_; }
    std::string to_str() { return "Model: " + this->model_ + " UART: " + std::to_string(this->uart_->get_baud_rate()) + " baud"; }

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
    uint8_t read_delay_ = 10;
    uint8_t read_retry_sleep_ = 10;
    uint8_t read_retry_attempts_ = 5;
};

class RegoBase : public PollingComponent {
public:
    void register_hub(RegoInterfaceComponent *hub){ this->hub_ = hub; }
    void set_rego_variable(std::uint16_t rego_variable) { this->rego_variable_ = rego_variable; }
    void update() override { }

protected:
    std::string int2hex(std::uint16_t value) {
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(sizeof(value)*2) << std::hex << value;
        return stream.str();
    }

    RegoInterfaceComponent *hub_;
    std::uint16_t rego_variable_;
};

}  // namespace rego
}  // namespace esphome
