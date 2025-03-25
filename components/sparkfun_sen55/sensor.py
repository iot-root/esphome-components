import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import (
    CONF_ID,
    CONF_UPDATE_INTERVAL,
    UNIT_MICROGRAMS_PER_CUBIC_METER,
    STATE_CLASS_MEASUREMENT,
)

# Namespaces
sparkfun_sen55_ns = cg.esphome_ns.namespace("sparkfun_sen55")
SEN55Component = sparkfun_sen55_ns.class_(
    "SEN55Component", cg.PollingComponent, i2c.I2CDevice
)

# YAML configuration keys
CONF_PM_1_0 = "pm_1_0"
CONF_PM_2_5 = "pm_2_5"
CONF_PM_4_0 = "pm_4_0"
CONF_PM_10_0 = "pm_10_0"
CONF_VOC_INDEX = "voc_index"
CONF_NOX_INDEX = "nox_index"

DEPENDENCIES = ["i2c"]

CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(SEN55Component).extend({
        cv.Optional(CONF_PM_1_0): sensor.sensor_schema(
            unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PM_2_5): sensor.sensor_schema(
            unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PM_4_0): sensor.sensor_schema(
            unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PM_10_0): sensor.sensor_schema(
            unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_VOC_INDEX): sensor.sensor_schema(
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_NOX_INDEX): sensor.sensor_schema(
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }).extend(i2c.i2c_device_schema(0x69)).extend({
        cv.Optional(CONF_UPDATE_INTERVAL, default="60s"): cv.update_interval,
    })
)

# Note: 'async def' for the new async codegen
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    # Must use 'await' for these functions in newer ESPHome
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))

    # Create each sensor if it's in the YAML config
    if CONF_PM_1_0 in config:
        pm1 = await sensor.new_sensor(config[CONF_PM_1_0])
        cg.add(var.set_pm_1_0_sensor(pm1))

    if CONF_PM_2_5 in config:
        pm25 = await sensor.new_sensor(config[CONF_PM_2_5])
        cg.add(var.set_pm_2_5_sensor(pm25))

    if CONF_PM_4_0 in config:
        pm4 = await sensor.new_sensor(config[CONF_PM_4_0])
        cg.add(var.set_pm_4_0_sensor(pm4))

    if CONF_PM_10_0 in config:
        pm10 = await sensor.new_sensor(config[CONF_PM_10_0])
        cg.add(var.set_pm_10_0_sensor(pm10))

    if CONF_VOC_INDEX in config:
        voc = await sensor.new_sensor(config[CONF_VOC_INDEX])
        cg.add(var.set_voc_index_sensor(voc))

    if CONF_NOX_INDEX in config:
        nox = await sensor.new_sensor(config[CONF_NOX_INDEX])
        cg.add(var.set_nox_index_sensor(nox))
