import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, CONF_PORT, CONF_BUFFER_SIZE

# ESPHome doesn't know the Stream abstraction yet, so hardcode to use a UART for now.

# AUTO_LOAD = ["socket"]

DEPENDENCIES = ["uart"]

MULTI_CONF = True

CONF_H60_INTERFACE = "h60_interface"
CONF_MODEL = "model"

ns = cg.global_ns
H60InterfaceComponent = ns.class_("H60InterfaceComponent", cg.Component)

def validate_buffer_size(buffer_size):
    if buffer_size & (buffer_size - 1) != 0:
        raise cv.Invalid("Buffer size must be a power of two.")
    return buffer_size


CONFIG_SCHEMA = cv.All(
    cv.require_esphome_version(2022, 3, 0),
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(H60InterfaceComponent),
            cv.Optional(CONF_MODEL, default="rego600"): cv.string,
            # cv.Optional(CONF_PORT, default=6638): cv.port,
            # cv.Optional(CONF_BUFFER_SIZE, default=128): cv.All(
            #     cv.positive_int, validate_buffer_size
            # ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_model(config[CONF_MODEL]))
    # cg.add(var.set_buffer_size(config[CONF_BUFFER_SIZE]))

    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)  # TODO: needed?


# async def to_code(config):
#     uart_component = await cg.get_variable(config[CONF_UART_ID])
#     var = cg.new_Pvariable(config[CONF_ID], uart_component, config[CONF_CRC_CHECK])
#     cg.add(var.set_max_telegram_length(config[CONF_MAX_TELEGRAM_LENGTH]))
#     if CONF_DECRYPTION_KEY in config:
#         cg.add(var.set_decryption_key(config[CONF_DECRYPTION_KEY]))
#     await cg.register_component(var, config)

#     if CONF_REQUEST_PIN in config:
#         request_pin = await cg.gpio_pin_expression(config[CONF_REQUEST_PIN])
#         cg.add(var.set_request_pin(request_pin))
#     cg.add(var.set_request_interval(config[CONF_REQUEST_INTERVAL].total_milliseconds))
#     cg.add(var.set_receive_timeout(config[CONF_RECEIVE_TIMEOUT].total_milliseconds))

#     cg.add_build_flag("-DDSMR_GAS_MBUS_ID=" + str(config[CONF_GAS_MBUS_ID]))

#     # DSMR Parser
#     cg.add_library("glmnet/Dsmr", "0.5")

#     # Crypto
#     cg.add_library("rweather/Crypto", "0.4.0")