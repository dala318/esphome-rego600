#include "rego_interface.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/util.h"
#include "esphome/core/version.h"

namespace esphome {
namespace rego {

static const char *TAG = "Rego";

void RegoBase::register_hub(RegoInterfaceComponent *hub) {
    this->hub_ = hub;
}

// void Parameter::publish_entities() {
//     // for (auto *binary_sensor : this->binary_sensors_){
//     //     binary_sensor->publish_state(this->b_value);
//     // }
//     // for (auto *sensor : this->sensors_){
//     //     sensor->publish_state(this->f_value);
//     // }
//     // for (auto *text_sensor : this->text_sensors_){
//     //     text_sensor->publish_state(this->t_value);
//     // }
// }

void RegoInterfaceComponent::set_model(std::string model) {
    this->model_ = model;

    // // This part should be uniqu for each model
    // this->parameters_.push_back(new Parameter(0x5122, "power"));
    // this->parameters_.push_back(new Parameter(0x9954, "heat_needed"));
    // this->parameters_.push_back(new Parameter(0x6845, "device_model"));
    // this->parameters_.push_back(new Parameter(0x1969, "return_temp"));

    // this->parameters_map_["power"] = new Parameter(0x5122, "power");
    // this->parameters_map_["heat_needed"] = new Parameter(0x9954, "heat_needed");
    // this->parameters_map_["device_model"] = new Parameter(0x6845, "device_model");
    // this->parameters_map_["return_temp"] = new Parameter(0x1969, "return_temp");
}

void RegoInterfaceComponent::setup() {
    ESP_LOGCONFIG(TAG, "Setting up Rego Interface...");

    // The make_unique() wrapper doesn't like arrays, so initialize the unique_ptr directly.
    // this->buf_ = std::unique_ptr<uint8_t[]>{new uint8_t[this->buf_size_]};
}

void RegoInterfaceComponent::loop() {
    // Simplified update interval assuming loop at 60Hz (16ms delta_time)
    // this->last_update_ += 16;
    // if (this->last_update_ >= this->update_interval_) {
    //     ESP_LOGCONFIG(TAG, "Executing in loop of component...");
    //     this->last_update_ = 0;
    //     this->write_registers();
    //     this->read_registers();
    // }
    this->write_registers();
    this->read_registers();
}

void RegoInterfaceComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Interface:");
    ESP_LOGCONFIG(TAG, "  UART device: %s", this->stream_);  // TODO: Need a "to_str" representation
    ESP_LOGCONFIG(TAG, "  Model: %s", this->model_);  // TODO: Need a "to_str" representation
    // for (auto *binary_sensor : this->binary_sensors_){
    //     LOG_BINARY_SENSOR("  ", "Binary sensor", binary_sensor);
    // }
    // for (auto *sensor : this->sensors_) {
    //     LOG_SENSOR("  ", "Sensor", sensor);
    // }
    // for (auto *text_sensor : this->text_sensors_){
    //     LOG_TEXT_SENSOR("  ", "Text sensor", text_sensor);
    // }
    // for (auto *switch_ : this->switches_){
    //     LOG_SWITCH("  ", "Switch", switch_);
    // }
}

void RegoInterfaceComponent::on_shutdown() {
}

// void RegoInterfaceComponent::register_binary_sensor(std::string id, RegoBinarySensor *obj) {
//     // this->binary_sensors_.push_back(obj);
//     // for (Parameter *parameter : this->parameters_){
//     //     if (parameter->identifier() == id) {
//     //         parameter->binary_sensors_.push_back(obj);
//     //     }
//     // }
// }

// void RegoInterfaceComponent::register_sensor(std::string id, RegoSensor *obj) {
//     // this->sensors_.push_back(obj);
//     // for (Parameter *parameter : this->parameters_){
//     //     if (parameter->identifier() == id) {
//     //         parameter->sensors_.push_back(obj);
//     //     }
//     // }
// }

// void RegoInterfaceComponent::register_text_sensor(std::string id, RegoTextSensor *obj) {
//     // this->text_sensors_.push_back(obj);
//     // for (Parameter *parameter : this->parameters_){
//     //     if (parameter->identifier() == id) {
//     //         parameter->text_sensors_.push_back(obj);
//     //     }
//     // }
// }

void RegoInterfaceComponent::write_registers() {
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

void RegoInterfaceComponent::read_registers() {
    size_t len = 0;
    int available;
    // while ((available = this->stream_->available()) > 0) {
    //     size_t free = this->buf_size_ - (this->buf_head_ - this->buf_tail_);
    //     if (free == 0) {
    //         // Only overwrite if nothing has been added yet, otherwise give flush() a chance to empty the buffer first.
    //         if (len > 0)
    //             return;

    //         ESP_LOGE(TAG, "Incoming bytes available, but outgoing buffer is full: stream will be corrupted!");
    //         free = std::min<size_t>(available, this->buf_size_);
    //         this->buf_tail_ += free;
    //     }

    //     // Fill all available contiguous space in the ring buffer.
    //     len = std::min<size_t>(available, std::min<size_t>(this->buf_ahead(this->buf_head_), free));
    //     this->stream_->read_array(&this->buf_[this->buf_index(this->buf_head_)], len);
    //     this->buf_head_ += len;
    // }

    // for (Parameter *parameter : this->parameters_){
    //     // parameter->
    // }

}


std::string RegoInterfaceComponent::read_value(int16_t reg, std::string name)
{
  uint16_t response = 0;
//   response = command_and_response(0x81, 0x02, reg, 0x00);
  // if error, try again
  if ((response >= 0x8000) && (response <= 0x800F)) {
    // response = command_and_response(0x81, 0x02, reg, 0x00);
  }
  std::string url = "Return value"; // TODO: Temporare return
//   std::string url = std::string(reg, HEX);
//   url += "=";
//   url += std::string(response, HEX);
//   Serial1.print(name + " ");
//   Serial1.print(reg,HEX);
//   Serial1.print("=");
//   Serial1.print(response,DEC);
//   Serial1.print(" (");
//   Serial1.print(response,HEX);
//   Serial1.println(" hex)");
  return url;
}

// int16_t RegoInterfaceComponent::command_and_response(std::byte addr, std::byte cmd, int16_t reg, int16_t val)
// {
//   std::byte request[9];
  
//   // Compose command
//   *request = addr;
//   *(request+1) = cmd;
//   *(request+8) = 0; // XOR
//   int2write(reg,request+2);
//   int2write(val,request+5);
//   for (int i=2; i< 8; i++)
//   {
//      *(request+8) ^= *(request+i); // update XOR with data
//   }

//   // Send command
//   ESP_LOGI("Command to send: " + hex2str(request, 9))  // TODO: How to merge all the bytes below into one string
//   //Serial1.print("Command to send: "); // send data to port
//   for (int i=0; i< 9; i++)
//   {
//      Serial.write(*(request+i)); // send data to port
//      //Serial1.print(*(request+i),HEX); // send data to port
//      //Serial1.print(" "); // send separator
//   }
//   //Serial1.println(" "); // end of line

//   // Read result
//   std::byte MyResponse[5];
//   int TO = 0;	// timeout value
//   int len = 0;	// count of received characters
//   bool done=false;	// are we done with reading?
//   do {
//     if (Serial.available() > 0) {
//       // read the incoming byte:
//       *(MyResponse+len) = Serial.read();
//     //   Serial1.print("R: ");
//     //   Serial1.println(*(response+cnt),HEX);
//       if (++len == 5) {done = true;}
//     } else {
//       delay(100);
//       if (++TO > 10) {done = true;}
//       //Serial1.print("T");
//     }
//   } while (!done);

//   int16_t res = 0x8001;
//   bool valid = (len == 5);
//   if ((valid) && (MyResponse[0]!=0x01)) {
//     valid=false; res = 0x8002;
//   }
//   if ((valid) && (MyResponse[4]!=(MyResponse[1]^MyResponse[2]^MyResponse[3]))) {
//     valid=false; res = 0x8003;
//   }
//   if (valid) {
//     res = read2int(MyResponse+1);
//   }
//   return res;
// }

}  // namespace rego
}  // namespace esphome