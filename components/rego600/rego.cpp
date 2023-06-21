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

void RegoInterfaceComponent::loop() {
    if (this->bussy_) {
        this->bussy_counter_++;
    }
    else {
        this->bussy_counter_ = 0;
        // Check the UART rx-buffer for received data that is not requested by any entity
        int available = 0;
        uint8_t response[128];
        if ((available = this->uart_->available()) > 0) {
            this->uart_->read_array(response, available);
            ESP_LOGI(TAG, "Data received from UART outside request: %s", data2hexstr(response, available).c_str());
        }
    }
    if (this->bussy_counter_ > 2000) {
        ESP_LOGD(TAG, "Max bussy_counter reached, resetting and enabling communication");
        this->bussy_ = false;
        this->bussy_counter_ = 0;
    }

    // this->write_registers();
    // this->read_registers();
}

bool RegoInterfaceComponent::read_value(int16_t reg, uint16_t *result)
{
    // ESP_LOGD(TAG, "Processing read of register %s (%u)", this->data2hexstr, data2hexstr(reg, sizeof(reg)).c_str(), reg);
    if (this->bussy_) {
        if (this->log_all_) {
            ESP_LOGI(TAG, "UART bus bussy (%u), more attempts will be made", this->bussy_counter_);
        }
        return false;
    }
    else if (this->log_all_) {
        ESP_LOGD(TAG, "UART bus free to use");
    }
    this->bussy_ = true;
    bool response = command_and_response(0x81, 0x02, reg, 0x00, result);
    this->bussy_ = false;
    return response;
}

bool RegoInterfaceComponent::write_value(int16_t reg, uint16_t value, uint16_t *result)
{
    // ESP_LOGD(TAG, "Processing write value %u to register %s (%u)", value, this->data2hexstr, data2hexstr(reg, sizeof(reg)).c_str(), reg);
    if (this->bussy_) {
        if (this->log_all_) {
            ESP_LOGI(TAG, "UART bus bussy (%u), more attempts will be made", this->bussy_counter_);
        }
        return false;
    }
    else if (this->log_all_) {
        ESP_LOGD(TAG, "UART bus free to use");
    }
    this->bussy_ = true;
    bool response = command_and_response(0x81, 0x03, reg, value, result);
    this->bussy_ = false;
    return response;
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

std::string RegoInterfaceComponent::data2hexstr(const uint8_t *data, size_t len)
{
    std::stringstream ss;
    ss << std::hex;
    for( int i(0) ; i < len; ++i )
        ss << std::setw(2) << std::setfill('0') << (int)data[i];
    return ss.str();
}

bool RegoInterfaceComponent::command_and_response(uint8_t addr, uint8_t cmd, uint16_t reg, uint16_t val, uint16_t *result)
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
    ESP_LOGD(TAG, "Command to send: %s", data2hexstr(request, sizeof(request)).c_str());
    this->uart_->write_array(request, sizeof(request));
    this->uart_->flush();

    // Read result
    delay(this->read_delay_);
    int available = 0;
    uint8_t response[128];
    uint8_t attempt = 0;
    while (attempt <= this->read_retry_attempts_) {
        if ((available = this->uart_->available()) > 0) {
            this->uart_->read_array(response, available);
            ESP_LOGD(TAG, "Response received: %s", data2hexstr(response, available).c_str());
            break;
        }
        attempt++;
        if (attempt <= this->read_retry_attempts_) {
            if (this->log_all_) {
                ESP_LOGD(TAG, "No response yet, sleeping %ums and retrying", this->read_retry_sleep_);
            }
            delay(this->read_retry_sleep_);
        }
    }
    if (available == 0) {
        ESP_LOGE(TAG, "No response after %u attempts", (attempt));
        return false;
    }

    // Decode response
    if (available != 5) {
        ESP_LOGE(TAG, "Response wrong size");
        return false;
    }
    if (response[0] != 0x01) {
        ESP_LOGE(TAG, "Response from wrong address");
        return false;
    }
    if (response[4] != (response[1]^response[2]^response[3])) {
        ESP_LOGE(TAG, "Response wrong checksum");
        return false;
    }
    // TODO: Add check that resonse address matches the request address
    *result = read2int(response+1);
    ESP_LOGD(TAG, "Response decoded to %u", result);
    return true;
}

}  // namespace rego
}  // namespace esphome