import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    STATE_CLASS_MEASUREMENT,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import ns, H60InterfaceComponent, CONF_H60_INTERFACE

CONF_POWER = "power"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_H60_INTERFACE): cv.use_id(H60InterfaceComponent),
        cv.Optional(CONF_POWER): sensor.sensor_schema(),
        # cv.Required(CONF_CONNECTION_COUNT): sensor.sensor_schema(
        #     accuracy_decimals=0,
        #     state_class=STATE_CLASS_MEASUREMENT,
        #     entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        # ),
    } # TODO: Add same as in binary_sensor
)

async def to_code(config):
    h60if = await cg.get_variable(config[CONF_H60_INTERFACE])

    sensors = []
    for key, conf in config.items():
        if not isinstance(conf, dict):
            continue
        id = conf.get("id")
        if id and id.type == sensor.Sensor:
            var = await sensor.new_sensor(conf)
            cg.add(getattr(h60if, f"set_{key}")(var))
            sensors.append(f"F({key})")

    if sensors:
        cg.add_define(
            "H60_SENSOR_LIST(F, sep)",
            cg.RawExpression(" sep ".join(sensors)),
        )
