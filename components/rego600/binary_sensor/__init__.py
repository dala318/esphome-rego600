import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
# from esphome.const import (
# )
from .. import ns, RegoInterfaceComponent, CONF_HUB_ID, REG_ADDR_SCHEMA

DEPENDENCIES = ['rego600']

binary_sensor_schema = binary_sensor.binary_sensor_schema(
    ns.class_(
        "RegoBinarySensor",
        binary_sensor.BinarySensor,
        cg.Component,
        ns.class_("RegoBase")
    )
).extend(cv.COMPONENT_SCHEMA)

CONF_DICT = {
    cv.Optional("connected"): binary_sensor_schema,
    cv.Optional("heat_needed"): binary_sensor_schema,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(RegoInterfaceComponent),
    }
).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await binary_sensor.new_binary_sensor(conf)
        await cg.register_component(var, conf)
        # cg.add(paren.register_binary_sensor(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)
