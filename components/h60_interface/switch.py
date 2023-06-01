import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
# from esphome.const import (
# )
from . import ns, H60InterfaceComponent, CONF_HUB_ID, CONF_PARAMETER_ID

DEPENDENCIES = ['h60_interface']

switch_ns = cg.esphome_ns.namespace('switch_')
Switch = switch_ns.class_('Switch', switch.Switch)
# Switch = switch_ns.class_('Switch', switch.Switch, cg.Nameable)

h60_ns = cg.esphome_ns.namespace("h60_interface")
CONF_DICT = {
    cv.Optional("additional_heat"): switch.switch_schema(h60_ns.class_("SwitchAdditionalHeat", switch.Switch, cg.Component)).extend(cv.COMPONENT_SCHEMA),
}

# CONF_SWITCH_PARAMETERS = ["power", "return_temp"]

# CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
#     cv.GenerateID(): cv.declare_id(Switch),
#     cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
#     cv.Required(CONF_PARAMETER_ID): cv.one_of(*CONF_SWITCH_PARAMETERS),
# }).extend(cv.COMPONENT_SCHEMA)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUB_ID): cv.use_id(H60InterfaceComponent),
    }
).extend(CONF_DICT)# .extend(cv.COMPONENT_SCHEMA)

async def setup_conf(paren, config, key):
    if key in config:
        conf = config[key]
        var = await switch.new_switch(conf)
        # await switch.register_switch(var, conf)
        await cg.register_component(var, conf)
        cg.add(paren.register_switch(str(key), var))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_HUB_ID])
    for key in CONF_DICT.keys():
        await setup_conf(paren, config, key)


# async def to_code(config):
#     paren = await cg.get_variable(config[CONF_HUB_ID])
#     var = cg.new_Pvariable(config[CONF_ID])
    
#     await switch.register_switch(var, config)
    
#     cg.add(paren.register_switch(var))

# async def to_code(config):
#     paren = await cg.get_variable(config[CONF_HUB_ID])

#     # for key, conf in config.items():
#     #     if not isinstance(conf, dict):
#     #         continue
#     #     id = conf.get("id")
#     #     if id and id.type == switch.Switch:
#     #         # var = cg.new_Pvariable(conf)
#     #         # await switch.register_switch(var, conf)
#     #         var = await switch.new_switch(conf)
#     #         cg.add(paren.register_switch(var))

#     for key, conf in config.items():
#         if not isinstance(conf, dict):
#             continue
#         id = conf.get("id")
#         if id and id.type == switch.Switch:
#             var = await switch.new_switch(conf)
#             cg.add(paren.register_switch(key, var))
