import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
# from esphome.const import (
# )
from . import ns, RegoInterfaceComponent, CONF_HUB_ID

DEPENDENCIES = ['rego600']

CONF_DICT = {
    cv.Optional("power"): sensor.sensor_schema(ns.class_("SensorPower", sensor.Sensor, cg.Component)).extend(cv.COMPONENT_SCHEMA),
    cv.Optional("return_temp"): sensor.sensor_schema(ns.class_("SensorReturnTemp", sensor.Sensor, cg.Component)).extend(cv.COMPONENT_SCHEMA),
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(RegoInterfaceComponent),
    }
).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await sensor.new_sensor(conf)
        await cg.register_component(var, conf)
        cg.add(paren.register_sensor(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)
