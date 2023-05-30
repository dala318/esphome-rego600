#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"

// #include "HD_Globals.h"
// #include "HD_Utilities.h"
// #include "HD_H1comm.h"

#include <memory>
#include <string>
#include <vector>

namespace esphome {
namespace h60_interface {

class H60InterfaceComponent : public esphome::Component {
public:
    H60InterfaceComponent() = default;
    explicit H60InterfaceComponent(esphome::uart::UARTComponent *stream) : stream_{stream} {}
    void set_model(std::string model) {this->model_ = model; }
    void set_uart_parent(esphome::uart::UARTComponent *parent) { this->stream_ = parent; }
    void set_buffer_size(size_t size) { this->buf_size_ = size; }

    // void set_connected_sensor(esphome::binary_sensor::BinarySensor *connected) { this->connected_sensor_ = connected; }
    // void set_connection_count_sensor(esphome::sensor::Sensor *connection_count) { this->connection_count_sensor_ = connection_count; }

    void setup() override;
    void loop() override;
    void dump_config() override;
    void on_shutdown() override;

    float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

protected:
    void publish_sensor();

    void accept();
    void cleanup();
    void read();
    void flush();
    void write();

    size_t buf_index(size_t pos) { return pos & (this->buf_size_ - 1); }
    size_t buf_ahead(size_t pos) { return (pos | (this->buf_size_ - 1)) - pos + 1; }

    struct Parameter {
        Parameter(std::int64_t reg, std::string identifier, size_t position);

        std::int64_t reg{}
        std::string identifier{};
        // bool disconnected{false};
        size_t position{0};
    };

    std::vector<Parameter> parameters_{};

    std::string model_;
    esphome::uart::UARTComponent *stream_{nullptr};
    size_t buf_size_;

    // esphome::binary_sensor::BinarySensor *connected_sensor_;
    // esphome::sensor::Sensor *connection_count_sensor_;

    std::unique_ptr<uint8_t[]> buf_{};
    size_t buf_head_{0};
    size_t buf_tail_{0};
};

}  // namespace h60_interface
}  // namespace esphome