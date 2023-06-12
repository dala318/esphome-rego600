#include "rego_interface.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/util.h"
#include "esphome/core/version.h"

namespace esphome {
namespace rego {

static const char *TAG = "Rego";

void RegoInterfaceComponent::loop() {
    this->write_registers();
    this->read_registers();
}

void RegoInterfaceComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Interface:");
    ESP_LOGCONFIG(TAG, "  UART device: %s", this->stream_);  // TODO: Need a "to_str" representation
    ESP_LOGCONFIG(TAG, "  Model: %s", this->model_);  // TODO: Need a "to_str" representation
}

void RegoInterfaceComponent::write_registers() {
}

void RegoInterfaceComponent::read_registers() {
    int available;
    // while ((available = this->stream_->available()) > 0) {
    if ((available = this->stream_->available()) > 0) {
        if (this->log_all_) {
            ESP_LOGD(TAG, "Spare data (%u bytes) avaialble on UART", available);
            // this->stream_->read_array(&this->buf_[this->buf_index(this->buf_head_)], len);
        }
        // TODO: Read and handle spare data on UART, take from StreamServer
        // Could be that this will interfere with the reading/writing from the entities and hence need to be removed
    }
}


std::string RegoInterfaceComponent::read_value(int16_t reg, std::string name)
{
  uint16_t response = 0;
//   response = command_and_response(0x81, 0x02, reg, 0x00);
  // if error, try again
  if ((response >= 0x8000) && (response <= 0x800F)) {
    // response = command_and_response(0x81, 0x02, reg, 0x00);
  }
  std::string url = "Return value"; // TODO: Temporary return
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

void RegoBase::register_hub(RegoInterfaceComponent *hub) {
    this->hub_ = hub;
}

}  // namespace rego
}  // namespace esphome