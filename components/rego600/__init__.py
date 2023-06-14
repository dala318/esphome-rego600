import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ['@dala318']

AUTO_LOAD = [ 'binary_sensor', 'sensor','text_sensor', 'button', 'climate', 'number', 'switch']
MULTI_CONF = True

DEPENDENCIES = ["uart"]

CONF_HUB_ID = "rego600_id"
CONF_MODEL = "model"
CONF_LOG_ALL = "log_all"
# CONF_UPDATE_INTERVAL = "update_interval"

# CONF_REG_ADDR = "reg_addr"
CONF_REGO_VARIABLE = "rego_variable"
CONF_VALUE_FACTOR = "value_factor"

ns = cg.esphome_ns.namespace("esphome::rego")
RegoInterfaceComponent = ns.class_("RegoInterfaceComponent", cg.Component)

# REG_ADDR_SCHEMA = cv.Schema({cv.Required(CONF_REG_ADDR): cv.hex_int})

CONFIG_SCHEMA = cv.All(
    # cv.require_esphome_version(2022, 3, 0),
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RegoInterfaceComponent),
            cv.Optional(CONF_MODEL, default="rego600"): cv.string,
            cv.Optional(CONF_LOG_ALL, default=False): cv.boolean,
            # cv.Optional(CONF_UPDATE_INTERVAL, default="2s"): cv.update_interval,
        }
    )
    # .extend(cv.COMPONENT_SCHEMA)
    # .extend(cv.polling_component_schema('10s'))
    .extend(uart.UART_DEVICE_SCHEMA),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_model(config[CONF_MODEL]))
    cg.add(var.set_log_all(config[CONF_LOG_ALL]))
    # cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))  # Seem to already be a part of component schema

    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)  # TODO: needed?
