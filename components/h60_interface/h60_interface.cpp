#include "h60_interface.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/util.h"
#include "esphome/core/version.h"

namespace esphome {
namespace h60_interface {

static const char *TAG = "h60_interface";

void H60InterfaceComponent::setup() {
    ESP_LOGCONFIG(TAG, "Setting up H60 Interface...");

    // The make_unique() wrapper doesn't like arrays, so initialize the unique_ptr directly.
    this->buf_ = std::unique_ptr<uint8_t[]>{new uint8_t[this->buf_size_]};

    // this->publish_sensors();
}

void H60InterfaceComponent::loop() {
    this->accept();
    this->read();
    this->flush();
    this->write();
    this->cleanup();
}

void H60InterfaceComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "H60 Interface:");
    for(auto *binary_sensor : this->binary_sensors_){
        LOG_BINARY_SENSOR("  ", "Binary sensor", binary_sensor);
    }
    for (auto *sensor : this->sensors_) {
        LOG_SENSOR("  ", "Sensor", sensor);
    }
    for(auto *text_sensor : this->text_sensors_){
        LOG_TEXT_SENSOR("  ", "Text sensor", text_sensor);
    }
}

void H60InterfaceComponent::on_shutdown() {
}

// void H60InterfaceComponent::publish_sensors() {
//     for (Parameter &parameter : this->parameters_) {   
//     }

//     // if (this->connected_sensor_)
//     //     // this->connected_sensor_->publish_state(this->clients_.size() > 0);
//     //     this->connected_sensor_->publish_state(this->port_ > 0);
//     // if (this->connection_count_sensor_)
//     //     // this->connection_count_sensor_->publish_state(this->clients_.size());
//     //     this->connection_count_sensor_->publish_state(this->port_);
// }

void H60InterfaceComponent::accept() {
    // this->publish_sensors();
}

void H60InterfaceComponent::read() {
    size_t len = 0;
    int available;
    while ((available = this->stream_->available()) > 0) {
        size_t free = this->buf_size_ - (this->buf_head_ - this->buf_tail_);
        if (free == 0) {
            // Only overwrite if nothing has been added yet, otherwise give flush() a chance to empty the buffer first.
            if (len > 0)
                return;

            ESP_LOGE(TAG, "Incoming bytes available, but outgoing buffer is full: stream will be corrupted!");
            free = std::min<size_t>(available, this->buf_size_);
            this->buf_tail_ += free;
        }

        // Fill all available contiguous space in the ring buffer.
        len = std::min<size_t>(available, std::min<size_t>(this->buf_ahead(this->buf_head_), free));
        this->stream_->read_array(&this->buf_[this->buf_index(this->buf_head_)], len);
        this->buf_head_ += len;
    }
}

void H60InterfaceComponent::flush() {
    ssize_t written;
    this->buf_tail_ = this->buf_head_;
}

void H60InterfaceComponent::write() {
    uint8_t buf[128];
    ssize_t read;
}

void H60InterfaceComponent::cleanup() {
}

}  // namespace h60_interface
}  // namespace esphome