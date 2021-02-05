README.txt
Farmer Frog Hoop House Controller System Instructions

###############################################################################
Sensor Side Board

There are 6 I2C inputs, which are designed to be used with Atlas Scientific
EZO Dissolved Oxygen (DO) sensors, but may work with similar sensors (with no
guarentees). When using other sensors, uploading custom code may be required.
There are 16 inputs which can read a digital binary value or an analog voltage.
These are configured using settings.txt (see settings.txt section of the
README) When using a digital sensor, if the sensor is open-collector, you must
solder a resistor in the designated location on the Sensor Side Board. See the
Sensor Board Setup section of the Repair Manual for more information.

###############################################################################
Relay Side Board



###############################################################################
settings.txt

I2Cx inputs are configured with either ON or OFF e.g.
OFF,I2C1 - OFF/ON
ON,I2C2 - OFF/ON
Where I2C1 is off and I2C2 is on.

INx inputs have five possible configurations: OFF, ANALOG, DIGITAL, BATTERY
OFF means that the input does not read anything.
ANALOG means that the sensor outputs an analog voltage between 0 and 3.3 volts
  This uses 12 bit ADCS on-board the ESP32-DEVKIT-V1.
DIGITAL means that the sensor outputs a digital binary signal, either 0 or 3.3
  volts, where 0 means that the sensor is reading true, and 3.3 means that the
  sensor is reading false (positive logic).
NDIGITAL means that the sensor outputs a digital binary signal, either 0 or 3.3
  volts, where 0 means that the sensor is reading false, and 3.3 means that the
  sensor is reading true (negative logic).
BATTERY means that the sensor is reading the battery level from the UPS system.
  ONLY SET ONE SIGNAL TO THE BATTERY SETTING. The system is only designed to
  use one battery level. See the UPS section of the Repair Manual for more
  information about this.

###############################################################################
data.txt

This file contains data from the sensors logged with time and date.