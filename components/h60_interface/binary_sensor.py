import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CONNECTIVITY,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import ns, H60InterfaceComponent, CONF_HUB_ID

DEPENDENCIES = ['h60_interface']

binary_sensor_ns = cg.esphome_ns.namespace('binary_sensor')
BinarySensor = binary_sensor_ns.class_('BinarySensor', binary_sensor.BinarySensor)
# BinarySensor = binary_sensor_ns.class_('BinarySensor', binary_sensor.BinarySensor, cg.Nameable)

CONF_CONNECTED = "connected"

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(BinarySensor),
    cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent)
}).extend(cv.COMPONENT_SCHEMA)


# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
#         cv.Optional(CONF_CONNECTED): binary_sensor.binary_sensor_schema(),
#         # cv.Optional("running"): binary_sensor.binary_sensor_schema(),
#         # cv.Required(CONF_CONNECTED): binary_sensor.binary_sensor_schema(
#         #     device_class=DEVICE_CLASS_CONNECTIVITY,
#         #     entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
#         # ),
#     }
# ).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    
    await binary_sensor.register_binary_sensor(var, config)
    
    cg.add(paren.register_binary_sensor(var))
