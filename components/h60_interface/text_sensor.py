import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CONNECTIVITY,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import ns, H60InterfaceComponent, CONF_HUB_ID, CONF_PARAMETER_ID

DEPENDENCIES = ['h60_interface']

h60_ns = cg.esphome_ns.namespace("h60_interface")
CONF_DICT = {
    cv.Optional("device_type"): text_sensor.text_sensor_schema(h60_ns.class_("TextSensorDeviceType", text_sensor.TextSensor, cg.PollingComponent)).extend(cv.COMPONENT_SCHEMA),
    cv.Optional("device_model"): text_sensor.text_sensor_schema(h60_ns.class_("TextSensorDeviceModel", text_sensor.TextSensor, cg.PollingComponent)).extend(cv.COMPONENT_SCHEMA),
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
    }
).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await text_sensor.new_text_sensor(conf)
        await cg.register_component(var, conf)
        cg.add(paren.register_text_sensor(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)
