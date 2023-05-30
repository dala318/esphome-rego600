#include "h60_interface.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/util.h"
#include "esphome/core/version.h"

static const char *TAG = "h60_interface";

namespace esphome {
namespace h60_interface {

void H60InterfaceComponent::setup() {
    ESP_LOGCONFIG(TAG, "Setting up H60 Interface...");

    // The make_unique() wrapper doesn't like arrays, so initialize the unique_ptr directly.
    this->buf_ = std::unique_ptr<uint8_t[]>{new uint8_t[this->buf_size_]};

    this->publish_sensor();
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
    // ESP_LOGCONFIG(TAG, "  Address: %s:%u", esphome::network::get_use_address().c_str(), this->port_);
    // LOG_BINARY_SENSOR("  ", "Connected:", this->connected_sensor_);
    // LOG_SENSOR("  ", "Connection count:", this->connection_count_sensor_);
}

void H60InterfaceComponent::on_shutdown() {
}

void H60InterfaceComponent::publish_sensor() {
    // if (this->connected_sensor_)
    //     // this->connected_sensor_->publish_state(this->clients_.size() > 0);
    //     this->connected_sensor_->publish_state(this->port_ > 0);
    // if (this->connection_count_sensor_)
    //     // this->connection_count_sensor_->publish_state(this->clients_.size());
    //     this->connection_count_sensor_->publish_state(this->port_);
}

void H60InterfaceComponent::accept() {
    this->publish_sensor();
}

void H60InterfaceComponent::cleanup() {
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

}  // namespace h60_interface
}  // namespace esphome