import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
# from esphome.const import (
# )
from . import ns, H60InterfaceComponent, CONF_HUB_ID, CONF_PARAMETER_ID

DEPENDENCIES = ['h60_interface']

h60_ns = cg.esphome_ns.namespace("h60_interface")
CONF_DICT = {
    cv.Optional("connected"): binary_sensor.binary_sensor_schema(h60_ns.class_("BinarySensorConnected", binary_sensor.BinarySensor, cg.Component)).extend(cv.COMPONENT_SCHEMA),
    cv.Optional("heat_needed"): binary_sensor.binary_sensor_schema(h60_ns.class_("BinarySensorHeatNeeded", binary_sensor.BinarySensor, cg.Component)).extend(cv.COMPONENT_SCHEMA),
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
    }
).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await binary_sensor.new_binary_sensor(conf)
        await cg.register_component(var, conf)
        cg.add(paren.register_binary_sensor(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)
