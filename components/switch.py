import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c']

# Define a namespace for your component
sparkfun_relay_ns = cg.esphome_ns.namespace('sparkfun_relay')
SparkFunRelaySwitch = sparkfun_relay_ns.class_('SparkFunRelaySwitch', switch.Switch, cg.Component)

# Register the configuration schema
CONFIG_SCHEMA = switch.switch_schema(SparkFunRelaySwitch).extend({}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var)
