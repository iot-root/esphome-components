import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]

# Create a namespace for the SparkFun Relay component.
sparkfun_relay_ns = cg.esphome_ns.namespace("sparkfun_relay")
SparkFunRelaySwitch = sparkfun_relay_ns.class_("SparkFunRelaySwitch", switch.Switch, cg.Component)

# Define the configuration schema. No extra configuration options are added here.
CONFIG_SCHEMA = switch.switch_schema(SparkFunRelaySwitch).extend({}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield switch.register_switch(var, config)
