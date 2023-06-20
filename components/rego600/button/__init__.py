import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID
from .. import ns, RegoInterfaceComponent, CONF_HUB_ID, CONF_REGO_VARIABLE, CONF_RETRY_WRITE, CONF_PAYLOAD

DEPENDENCIES = ['rego600']

RegoButton = ns.class_("RegoButton", button.Button, cg.PollingComponent)
CONFIG_SCHEMA = button.BUTTON_SCHEMA.extend( 
    {
        cv.GenerateID(): cv.declare_id(RegoButton),
        cv.GenerateID(CONF_HUB_ID): cv.use_id(RegoInterfaceComponent),
        cv.Required(CONF_REGO_VARIABLE): cv.hex_uint16_t,
        cv.Optional(CONF_PAYLOAD, default=0): cv.hex_uint16_t,
        cv.Optional(CONF_RETRY_WRITE, default=0): cv.uint8_t,
    }
).extend(cv.polling_component_schema('10s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await button.register_button(var, config)
    cg.add(var.set_rego_variable(int(config[CONF_REGO_VARIABLE])))
    cg.add(var.set_action_payload(int(config[CONF_PAYLOAD])))
    cg.add(var.set_retry_write(int(config[CONF_RETRY_WRITE])))
    paren = await cg.get_variable(config[CONF_HUB_ID])
    cg.add(var.register_hub(paren))
