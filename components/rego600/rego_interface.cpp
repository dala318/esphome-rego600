#include "rego_interface.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/util.h"
#include "esphome/core/version.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego_interface";

void Parameter::publish_entities() {
    for (auto *binary_sensor : this->binary_sensors_){
        binary_sensor->publish_state(this->b_value);
    }
    for (auto *sensor : this->sensors_){
        sensor->publish_state(this->f_value);
    }
    for (auto *text_sensor : this->text_sensors_){
        text_sensor->publish_state(this->t_value);
    }
}

void RegoInterfaceComponent::set_model(std::string model) {
    this->model_ = model;

    // This part should be uniqu for each model
    this->parameters_.push_back(new Parameter(0x5122, "power"));
    this->parameters_.push_back(new Parameter(0x9954, "heat_needed"));
    this->parameters_.push_back(new Parameter(0x6845, "device_model"));
    this->parameters_.push_back(new Parameter(0x1969, "return_temp"));

    this->parameters_map_["power"] = new Parameter(0x5122, "power");
    this->parameters_map_["heat_needed"] = new Parameter(0x9954, "heat_needed");
    this->parameters_map_["device_model"] = new Parameter(0x6845, "device_model");
    this->parameters_map_["return_temp"] = new Parameter(0x1969, "return_temp");
}

void RegoInterfaceComponent::setup() {
    ESP_LOGCONFIG(TAG, "Setting up Rego Interface...");

    // The make_unique() wrapper doesn't like arrays, so initialize the unique_ptr directly.
    this->buf_ = std::unique_ptr<uint8_t[]>{new uint8_t[this->buf_size_]};
}

void RegoInterfaceComponent::loop() {
    // Simplified update interval assuming loop at 60Hz (16ms delta_time)
    this->last_update_ += 16;
    if (this->last_update_ >= this->update_interval_) {
        this->last_update_ = 0;

        this->accept();
        this->read();
        this->flush();
        this->write();
        this->cleanup();
    }

    // this->loop_counter++;
    // for (auto *sensor : this->sensors_) {
    //     sensor->publish_state(this->loop_counter);
    // }
}

void RegoInterfaceComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Interface:");
    for (auto *binary_sensor : this->binary_sensors_){
        LOG_BINARY_SENSOR("  ", "Binary sensor", binary_sensor);
    }
    for (auto *sensor : this->sensors_) {
        LOG_SENSOR("  ", "Sensor", sensor);
    }
    for (auto *text_sensor : this->text_sensors_){
        LOG_TEXT_SENSOR("  ", "Text sensor", text_sensor);
    }
    for (auto *switch_ : this->switches_){
        LOG_SWITCH("  ", "Switch", switch_);
    }
}

void RegoInterfaceComponent::on_shutdown() {
}

void RegoInterfaceComponent::register_binary_sensor(std::string id, binary_sensor::BinarySensor *obj) {
    this->binary_sensors_.push_back(obj);
    for (Parameter *parameter : this->parameters_){
        if (parameter->identifier() == id) {
            parameter->binary_sensors_.push_back(obj);
        }
    }
}

void RegoInterfaceComponent::register_sensor(std::string id, sensor::Sensor *obj) {
    this->sensors_.push_back(obj);
    for (Parameter *parameter : this->parameters_){
        if (parameter->identifier() == id) {
            parameter->sensors_.push_back(obj);
        }
    }
}

void RegoInterfaceComponent::register_text_sensor(std::string id, text_sensor::TextSensor *obj) {
    this->text_sensors_.push_back(obj);
    for (Parameter *parameter : this->parameters_){
        if (parameter->identifier() == id) {
            parameter->text_sensors_.push_back(obj);
        }
    }
}

void RegoInterfaceComponent::accept() {
    // this->publish_sensors();
}

void RegoInterfaceComponent::read() {
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

    // TODO: Remove, for testing only
    if (this->loop_counter > 1000) {
        this->loop_counter = -200;
    }
    for (Parameter *parameter : this->parameters_){
        this->loop_counter++;
        parameter->update_values(this->loop_counter);
    }
}

void RegoInterfaceComponent::flush() {
    ssize_t written;
    this->buf_tail_ = this->buf_head_;
}

void RegoInterfaceComponent::write() {
    uint8_t buf[128];
    ssize_t read;
    // for (Client &client : this->clients_) {
    //     if (client.disconnected)
    //         continue;

    //     while ((read = client.socket->read(&buf, sizeof(buf))) > 0)
    //         this->stream_->write_array(buf, read);

    //     if (read == 0 || errno == ECONNRESET) {
    //         ESP_LOGD(TAG, "Client %s disconnected", client.identifier.c_str());
    //         client.disconnected = true;
    //     } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
    //         // Expected if the (TCP) receive buffer is empty, nothing to do.
    //     } else {
    //         ESP_LOGW(TAG, "Failed to read from client %s with error %d!", client.identifier.c_str(), errno);
    //     }
    // }
}

void RegoInterfaceComponent::cleanup() {
}

}  // namespace rego
}  // namespace esphome