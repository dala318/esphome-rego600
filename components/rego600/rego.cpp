#include "rego.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/util.h"
#include "esphome/core/version.h"

namespace esphome {
namespace rego {

static const char *TAG = "rego";

void RegoInterfaceComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "Rego Interface:");
    ESP_LOGCONFIG(TAG, "  UART device: %s", this->uart_);  // TODO: Need a "to_str" representation
    // ESP_LOGCONFIG(TAG, "  UART device: %s", std::to_string(this->uart_->get_baud_rate()));
    ESP_LOGCONFIG(TAG, "  Model: %s", this->model_);  // TODO: Need a "to_str" representation
}

// void RegoInterfaceComponent::loop() {
//     this->write_registers();
//     this->read_registers();
// }

// void RegoInterfaceComponent::write_registers() {
// }

// void RegoInterfaceComponent::read_registers() {
//     int available;
//     // while ((available = this->uart_->available()) > 0) {
//     if ((available = this->uart_->available()) > 0) {
//         if (this->log_all_) {
//             ESP_LOGD(TAG, "Spare data (%u bytes) avaialble on UART", available);
//             // this->uart_->read_array(&this->buf_[this->buf_index(this->buf_head_)], len);
//         }
//         // TODO: Read and handle spare data on UART, take from StreamServer
//         // Could be that this will interfere with the reading/writing from the entities and hence need to be removed
//     }
// }


uint16_t RegoInterfaceComponent::read_value(int16_t reg)
{
    return command_and_response(0x81, 0x02, reg, 0x00);
}

void RegoInterfaceComponent::int2write (int16_t value, uint8_t *write_array)
{
    *(write_array+2) = (uint8_t)value & 0x7F;
    *(write_array+1) = (uint8_t)(value>>7) & 0x7F;
    *write_array = (uint8_t)(value>>14) & 0x03;
}

int16_t RegoInterfaceComponent::read2int(uint8_t *read_array)
{
    int16_t res = ( (int16_t)*(read_array) << 14 ) | ( (int16_t)*(read_array+1) << 7 ) | (int16_t)*(read_array+2);
    return res;
}

std::string RegoInterfaceComponent::hex2str(const uint8_t *data, size_t len)
{
    std::stringstream ss;
    ss << std::hex;
    for( int i(0) ; i < len; ++i )
        ss << std::setw(2) << std::setfill('0') << (int)data[i];
    return ss.str();
}

int16_t RegoInterfaceComponent::command_and_response(uint8_t addr, uint8_t cmd, uint16_t reg, uint16_t val)
{
    // Compose command
    uint8_t request[9];
    *request = addr;
    *(request+1) = cmd;
    int2write(reg,request+2);
    int2write(val,request+5);
    *(request+8) = 0;
    for (int i=2; i< 8; i++)
    {
        *(request+8) ^= *(request+i); // update XOR with data
    }

    // Send command
    ESP_LOGD(TAG, "Command to send: %s", hex2str(request, sizeof(request)).c_str());  // TODO: How to merge all the bytes below into one string
    this->uart_->write_array(request, sizeof(request));

    // Read result
    int available = 0;
    uint8_t response[128];
    while ((available = this->uart_->available()) > 0) {
        this->uart_->read_array(response, available);
        ESP_LOGD(TAG, "Response received: %s", hex2str(response, available).c_str());  // TODO: How to merge all the bytes below into one string
    }

    // Decode response
    int16_t result = 0x8001;
    bool valid = (available == 5);
    if ((valid) && (response[0]!=0x01)) {
        valid=false; result = 0x8002;
        ESP_LOGE(TAG, "Response from wrong address");
    }
    if ((valid) && (response[4]!=(response[1]^response[2]^response[3]))) {
        valid=false; result = 0x8003;
        ESP_LOGE(TAG, "Response wrong checksum");
    }
    if (valid) {
        result = read2int(response+1);
        ESP_LOGD(TAG, "Response decoded to %u", result);
    }
    else {
        ESP_LOGE(TAG, "Response wrong size");
    }
    return result;
}

}  // namespace rego
}  // namespace esphome