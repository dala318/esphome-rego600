import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    DEVICE_CLASS_CONNECTIVITY,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import ns, H60InterfaceComponent, CONF_H60_INTERFACE

CONF_CONNECTED = "connected"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_H60_INTERFACE): cv.use_id(H60InterfaceComponent),
        cv.Optional(CONF_CONNECTED): binary_sensor.binary_sensor_schema(),
        # cv.Optional("running"): binary_sensor.binary_sensor_schema(),
        # cv.Required(CONF_CONNECTED): binary_sensor.binary_sensor_schema(
        #     device_class=DEVICE_CLASS_CONNECTIVITY,
        #     entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        # ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    h60if = await cg.get_variable(config[CONF_H60_INTERFACE])

    # sens = await binary_sensor.new_binary_sensor(config[CONF_CONNECTED])
    # cg.add(h60if.set_connected_sensor(sens))

    binary_sensors = []
    for key, conf in config.items():
        if not isinstance(conf, dict):
            continue
        id = conf.get("id")
        if id and id.type == binary_sensors.BinarySensor:
            var = await binary_sensors.new_binary_sensor(conf)
            cg.add(getattr(h60if, f"set_{key}")(var))
            binary_sensors.append(f"F({key})")

    if binary_sensors:
        cg.add_define(
            "H60_BINARY_SENSOR_LIST(F, sep)",
            cg.RawExpression(" sep ".join(binary_sensors)),
        )
