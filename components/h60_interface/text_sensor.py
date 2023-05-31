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

text_sensor_ns = cg.esphome_ns.namespace('text_sensor')
TextSensor = text_sensor_ns.class_('TextSensor', text_sensor.TextSensor)
# TextSensor = text_sensor_ns.class_('TextSensor', text_sensor.TextSensor, cg.Nameable)

# CONF_DEVICE_TYPE = "device_type"
# CONF_DEVICE_MODEL = "device_model"
CONF_TEXT_SENSOR_PARAMETERS = ["device_type", "device_model"]

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(TextSensor),
    cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
    cv.Required(CONF_PARAMETER_ID): cv.one_of(*CONF_TEXT_SENSOR_PARAMETERS),
}).extend(cv.COMPONENT_SCHEMA)

# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
#         cv.Optional(CONF_DEVICE_TYPE): text_sensor.text_sensor_schema(),
#         cv.Optional(CONF_DEVICE_MODEL): text_sensor.text_sensor_schema(),
#         # cv.Optional("running"): text_sensor.text_sensor_schema(),
#         # cv.Required(CONF_CONNECTED): text_sensor.text_sensor_schema(
#         #     device_class=DEVICE_CLASS_CONNECTIVITY,
#         #     entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
#         # ),
#     }
# ).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    var = cg.new_Pvariable(config[CONF_ID])

    await text_sensor.register_text_sensor(var, config)

    cg.add(paren.register_text_sensor(var))
