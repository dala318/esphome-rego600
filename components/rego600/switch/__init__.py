import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
# from esphome.const import (
# )
from .. import ns, RegoInterfaceComponent, CONF_HUB_ID

DEPENDENCIES = ['rego600']

switch_schema = switch.switch_schema(
    ns.class_(
        "RegoSwitch",
        switch.Switch,
        cg.Component,
        ns.class_("RegoBase")
    )
).extend(cv.COMPONENT_SCHEMA)

CONF_DICT = {
    cv.Optional("additional_heat"): switch_schema,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(RegoInterfaceComponent),
    }
).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await switch.new_switch(conf)
        await cg.register_component(var, conf)
        cg.add(var.register_hub(paren))
        # cg.add(paren.register_switch(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)
