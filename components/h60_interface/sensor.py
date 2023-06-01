import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    UNIT_EMPTY,
    ICON_EMPTY,
    STATE_CLASS_MEASUREMENT,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import ns, H60InterfaceComponent, CONF_HUB_ID, CONF_PARAMETER_ID

DEPENDENCIES = ['h60_interface']

sensor_ns = cg.esphome_ns.namespace('sensor')
Sensor = sensor_ns.class_('Sensor', sensor.Sensor)
# Sensor = sensor_ns.class_('Sensor', sensor.Sensor, cg.Nameable)

DEFAULT_UPDATE_INTERVAL = "5s"

h60_ns = cg.esphome_ns.namespace("h60_interface")
# CONF_POWER = "power"
# CONF_RETURN_TEMP = "return_temp"
CONF_DICT = {
    "power": h60_ns.class_("SensorPower", sensor.Sensor, cg.PollingComponent),
    "return_temp": h60_ns.class_("SensorReturnTemp", sensor.Sensor, cg.PollingComponent)
}
CONF_DICT = {
    cv.Optional("power"): sensor.sensor_schema(h60_ns.class_("SensorPower", sensor.Sensor, cg.PollingComponent)).extend(cv.COMPONENT_SCHEMA),
    cv.Optional("return_temp"): sensor.sensor_schema(h60_ns.class_("SensorReturnTemp", sensor.Sensor, cg.PollingComponent)).extend(cv.COMPONENT_SCHEMA),
}
# SensorPower = h60_ns.class_("SensorPower", sensor.Sensor, cg.PollingComponent)
# SensorReturnTemp = h60_ns.class_("SensorReturnTemp", sensor.Sensor, cg.PollingComponent)

# CONF_SENSOR_PARAMETERS = ["power", "return_temp"]
# CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend({
#     cv.GenerateID(): cv.declare_id(Sensor),
#     cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
#     cv.Required(CONF_PARAMETER_ID): cv.one_of(*CONF_SENSOR_PARAMETERS),
# }).extend(cv.COMPONENT_SCHEMA)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
        # cv.Optional(CONF_POWER): sensor.sensor_schema(SensorPower)
        #     .extend(cv.COMPONENT_SCHEMA),
        # cv.Optional(CONF_RETURN_TEMP): sensor.sensor_schema(SensorReturnTemp)
        #     .extend(cv.COMPONENT_SCHEMA),
    }
# )# .extend(cv.COMPONENT_SCHEMA)
).extend(CONF_DICT)
# for key, val in CONF_DICT.items():
#     CONFIG_SCHEMA.extend({cv.Optional(key): sensor.sensor_schema(val).extend(cv.COMPONENT_SCHEMA)})

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await sensor.new_sensor(conf)
        # await sensor.register_sensor(var, conf)
        await cg.register_component(var, conf)
        cg.add(paren.register_sensor(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)

    # await setup_conf(paren, config, CONF_POWER)
    # await setup_conf(paren, config, CONF_RETURN_TEMP)


# async def to_code(config):
#     paren = await cg.get_variable(config[CONF_HUB_ID])
#     var = cg.new_Pvariable(config[CONF_ID])
    
#     await sensor.register_sensor(var, config)
    
#     cg.add(paren.register_sensor(var))

# async def to_code(config):
#     paren = await cg.get_variable(config[CONF_HUB_ID])

#     # for key, conf in config.items():
#     #     if not isinstance(conf, dict):
#     #         continue
#     #     id = conf.get("id")
#     #     if id and id.type == sensor.Sensor:
#     #         # var = cg.new_Pvariable(conf)
#     #         # await sensor.register_sensor(var, conf)
#     #         var = await sensor.new_sensor(conf)
#     #         cg.add(paren.register_sensor(var))

#     for key, conf in config.items():
#         if not isinstance(conf, dict):
#             continue
#         id = conf.get("id")
#         if id and id.type == sensor.Sensor:
#             var = await sensor.new_sensor(conf)
#             cg.add(paren.register_sensor(key, var))
