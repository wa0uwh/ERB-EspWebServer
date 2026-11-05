# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261104l
# erb_heap_log - on-device free-heap history: NVS ring (no partition change), served at /heap_history.csv
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["web_server_base", "esp32"]
AUTO_LOAD = ["web_server_base"]
erb_ns = cg.esphome_ns.namespace("erb_heap_log")
ErbHeapLog = erb_ns.class_("ErbHeapLog", cg.Component)

CONFIG_SCHEMA = cv.Schema({cv.GenerateID(): cv.declare_id(ErbHeapLog)}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
# End
