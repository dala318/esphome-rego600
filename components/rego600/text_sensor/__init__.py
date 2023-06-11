import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
# from esphome.const import (
# )
from .. import ns, RegoInterfaceComponent, CONF_HUB_ID

DEPENDENCIES = ['rego600']

text_sensor_schema = text_sensor.text_sensor_schema(
    ns.class_(
        "RegoTextSensor",
        text_sensor.TextSensor,
        cg.PollingComponent,
        ns.class_("RegoBase")
    )
).extend(cv.polling_component_schema('10s'))

CONF_DICT = {
    cv.Optional("device_type"): text_sensor_schema,
    cv.Optional("device_model"): text_sensor_schema,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(RegoInterfaceComponent),
    }
).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await text_sensor.new_text_sensor(conf)
        await cg.register_component(var, conf)
        cg.add(var.register_hub(paren))
        # cg.add(paren.register_text_sensor(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)
