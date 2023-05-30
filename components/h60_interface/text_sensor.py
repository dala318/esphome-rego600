import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    DEVICE_CLASS_CONNECTIVITY,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import ns, H60InterfaceComponent, CONF_H60_INTERFACE

CONF_DEVICE_TYPE = "device_type"
CONF_DEVICE_MODEL = "device_model"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_H60_INTERFACE): cv.use_id(H60InterfaceComponent),
        cv.Optional(CONF_DEVICE_TYPE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_DEVICE_MODEL): text_sensor.text_sensor_schema(),
        # cv.Optional("running"): text_sensor.text_sensor_schema(),
        # cv.Required(CONF_CONNECTED): text_sensor.text_sensor_schema(
        #     device_class=DEVICE_CLASS_CONNECTIVITY,
        #     entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        # ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    h60if = await cg.get_variable(config[CONF_H60_INTERFACE])

    # sens = await text_sensor.new_text_sensor(config[CONF_CONNECTED])
    # cg.add(h60if.set_connected_sensor(sens))

    text_sensors = []
    for key, conf in config.items():
        if not isinstance(conf, dict):
            continue
        id = conf.get("id")
        if id and id.type == text_sensor.TextSensor:
            var = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(h60if, f"set_{key}")(var))
            text_sensors.append(f"F({key})")

    if text_sensors:
        cg.add_define(
            "H60_TEXT_SENSOR_LIST(F, sep)",
            cg.RawExpression(" sep ".join(text_sensors)),
        )
