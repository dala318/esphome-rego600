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
from . import ns, H60InterfaceComponent, CONF_HUB_ID

DEPENDENCIES = ['h60_interface']

sensor_ns = cg.esphome_ns.namespace('sensor')
Sensor = sensor_ns.class_('Sensor', sensor.Sensor)
# Sensor = sensor_ns.class_('Sensor', sensor.Sensor, cg.Nameable)

CONF_POWER = "power"

CONFIG_SCHEMA = sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1).extend({
    cv.GenerateID(): cv.declare_id(Sensor),
    cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent)
}).extend(cv.COMPONENT_SCHEMA)

# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
#         cv.Optional(CONF_POWER): sensor.sensor_schema(),
#         # cv.Required(CONF_CONNECTION_COUNT): sensor.sensor_schema(
#         #     accuracy_decimals=0,
#         #     state_class=STATE_CLASS_MEASUREMENT,
#         #     entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
#         # ),
#     } # TODO: Add same as in binary_sensor
# )

async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    
    await sensor.register_sensor(var, config)
    
    cg.add(paren.register_sensor(var))
