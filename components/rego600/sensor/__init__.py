import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
# from esphome.const import (
# )
from .. import ns, RegoInterfaceComponent, CONF_HUB_ID

DEPENDENCIES = ['rego600']

sensor_schema = sensor.sensor_schema(
    ns.class_(
        "RegoSensor",
        sensor.Sensor,
        cg.PollingComponent,
    )
).extend(cv.polling_component_schema('10s'))

CONF_DICT = {
    cv.Optional("power"): sensor_schema,
    cv.Optional("return_temp"): sensor_schema,
}

CONFIG_SCHEMA = cv.All(
    cv.has_at_least_one_key(*CONF_DICT.keys()),
    cv.Schema(
        {
            cv.GenerateID(CONF_HUB_ID): cv.use_id(RegoInterfaceComponent),
        }
    ).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)
)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await sensor.new_sensor(conf)
        await cg.register_component(var, conf)
        cg.add(var.register_hub(paren))
        # cg.add(paren.register_sensor(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)
