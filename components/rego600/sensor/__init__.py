import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from .. import ns, RegoInterfaceComponent, CONF_HUB_ID, CONF_REGO_VARIABLE, CONF_VALUE_FACTOR, not_zero_or_small

DEPENDENCIES = ['rego600']

RegoSensor = ns.class_("RegoSensor", sensor.Sensor, cg.PollingComponent)
CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend( 
    {
        cv.GenerateID(): cv.declare_id(RegoSensor),
        cv.GenerateID(CONF_HUB_ID): cv.use_id(RegoInterfaceComponent),
        cv.Required(CONF_REGO_VARIABLE): cv.hex_uint16_t,
        cv.Optional(CONF_VALUE_FACTOR, default=1.): cv.All(cv.float_, not_zero_or_small),
    }
).extend(cv.polling_component_schema('10s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    cg.add(var.set_rego_variable(int(config[CONF_REGO_VARIABLE])))
    cg.add(var.set_value_factor(config[CONF_VALUE_FACTOR]))
    paren = await cg.get_variable(config[CONF_HUB_ID])
    cg.add(var.register_hub(paren))
